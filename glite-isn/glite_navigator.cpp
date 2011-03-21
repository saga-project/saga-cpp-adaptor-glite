/*
 * Copyright (c) Members of the EGEE Collaboration. 2009-2010.
 * See http://www.eu-egee.org/partners/ for details on the copyright
 * holders.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <vector>
#include <string>

#include <stdlib.h>

#include <saga/util.hpp>
#include <saga/url.hpp>
#include <saga/adaptors/task.hpp>
#include <saga/adaptors/attribute.hpp>

#include <impl/config.hpp>
#include "glite_navigator.hpp"

#include "ldap_provider.hpp"

#include "saga/saga/packages/isn/entity_types.hpp"

namespace glite_isn_adaptor {

navigator_cpi_impl::navigator_cpi_impl (proxy* p,
                                        cpi_info       const & info,
                                        saga::ini::ini const & glob_ini,
                                        saga::ini::ini const & adap_ini,
                                        TR1::shared_ptr<saga::adaptor> adaptor)
   : base_cpi (p, info, adaptor, cpi::Noflags)
{
   // TODO: extract info-provider, host/port etc.
   //port = 2170;

   saga::session s = p->get_session();
   boost::shared_ptr<saga::session> sess(new saga::session(s.clone())); 
   set_session(sess);

   std::vector<saga::context> ctxs(s.list_contexts());
   std::vector<saga::context>::const_iterator iter;
   std::vector<saga::context>::const_iterator end = ctxs.end();

   //Loop through all the contexts looking for
   //the glite one
   for ( iter = ctxs.begin(); iter != end; ++iter )
   {
      if ( (iter->attribute_exists(saga::attributes::context_type)) &&
           ("glite" ==
               iter->get_attribute(saga::attributes::context_type)) )
      {
         //Get the location of the user proxy certificate
         //if it's in the context
         if ( iter->attribute_exists(saga::attributes::context_userproxy) )
         {
            _proxy_location =
               iter->get_attribute(saga::attributes::context_userproxy);
         }
      }
   }

   instance_data data(this);
   std::string loc(data->location_.get_url());

   //Do we have an URL for the LDAP server?
   if (loc.empty() )
   {
    const char* env = saga::detail::safe_getenv("BDII_URL");
    if (env)
    {
      loc = env;
    }
    else
    {
      env = saga::detail::safe_getenv("LCG_GFAL_INFOSYS");
      if (env)
      {
        loc = "ldap://" + std::string(env);
      }
      else
      {
        loc = "ldap://lcg-bdii.cern.ch:2170";
      }
    }
  }
   
   _ldap = new ldap_provider(loc);

   //Set up the config directory from the model
   std::string model = data->model_;

   std::transform(model.begin(),
                  model.end(),
                  model.begin(),
                  static_cast<int(*)(int)>(std::tolower));

   std::string configDirectory = "/usr/etc/saga/isn/";
   if ( model == "glue1" )
   {
      configDirectory += "glue1/";
   }

   else if ( model == "glue2" )
   {
      configDirectory += "glue2/";
   }

   else
   {
      SAGA_THROW("Information System model '" + model + "' unknown",
                 saga::BadParameter);
   }
   _configDirectory = configDirectory;
   _model = model;
}

navigator_cpi_impl::~navigator_cpi_impl (void)
{
   delete _ldap;
}

void navigator_cpi_impl::set_session(const boost::shared_ptr<saga::session> s)
{
   _session = s;
}

void navigator_cpi_impl::
   sync_get_entities(std::vector<saga::isn::entity_data>& ret,
                     std::string entityType,
                     std::string filter)
{
   ENTITY_ATTR_TYPE attr =
      config_handler::get_entity(entityType, _configDirectory);

   std::string objectClass = attr.adaptorName;

   std::vector<std::string> filterVec;
   filterVec.push_back("(ObjectClass=" + objectClass + ")");

   std::vector<saga::isn::entity_data> entities =
      _ldap->get_entities(_model,
                          filterVec,
                          attr.attrs);

   //Now apply the filter to the entities
   std::vector<saga::isn::entity_data>::iterator iter = entities.begin();

   if ( filter.empty() == false )
   {
      bdii_query data_filter(_model, filter, entityType);

      std::vector<saga::isn::entity_data> filteredEntities;

      while ( iter != entities.end() )
      {
         bool selectEntity =
            data_filter.evaluate_data_filter(*iter);

         if ( selectEntity == true )
         {
            //add this element to the temp vector
            filteredEntities.push_back(*iter);
         }
         ++iter;
      }

      ret = filteredEntities;
      return;
   }

   else
   {
      ret = entities;
      return;
   }
}

void navigator_cpi_impl::
   sync_list_related_entity_names(std::vector<std::string>& ret,
                                  std::string entity)
{
   //Get all the details for the passed entity type
   ENTITY_ATTR_TYPE attr =
      config_handler::get_entity(entity, _configDirectory);

   ret.clear();

   std::multimap<std::string, ENTITY_RELATIONSHIP_TYPE>::const_iterator iter;
   iter = attr.relatedEntities.begin();

   //Add all the related entities to our returned vector
   while ( iter != attr.relatedEntities.end() )
   {
      ret.push_back(iter->first);
      ++iter;
   }
   return;
}

void navigator_cpi_impl::
   sync_get_related_entities(std::vector<saga::isn::entity_data>& ret,
                             std::string entityName,
                             std::string relatedEntityName,
                             std::string filter,
                             std::vector<saga::isn::entity_data> entities)
{
   //Get the details about this entity type from the config_handler
   ENTITY_ATTR_TYPE attr =
      config_handler::get_entity(entityName, _configDirectory);

   //Get the details about the other entity type from the config_handler
   ENTITY_ATTR_TYPE relAttr =
      config_handler::get_entity(relatedEntityName, _configDirectory);

   std::multimap<std::string, ENTITY_RELATIONSHIP_TYPE>::const_iterator
      relIter = attr.relatedEntities.find(relatedEntityName);

   //Check this relationship exists
   if ( relIter == attr.relatedEntities.end() )
   {
      std::string err("Unknown relationship: ");
      err += relatedEntityName;
      SAGA_THROW(err, saga::BadParameter);
   }

   //The pairs contain the attribute name (which we need to find)
   //and the adaptor representation of it.
   std::pair<std::string, std::string> primaryKey;
   primaryKey.second = relIter->second.primaryKey;

   std::pair<std::string, std::string> secondaryKey;
   secondaryKey.second = relIter->second.secondaryKey;

   bool reverse = relIter->second.reverseLookup;

   std::map<std::string, ENTITY_ATTR_MAP_TYPE>::const_iterator attIter;

   if ( reverse == false )
   {
      //Not a reverse lookup so primary is the current entity and
      //secondary is the related entity

      //Loop through this map until we find the adaptor version of the
      //attribute name in the data and then see what the key is
      attIter = attr.attrs.begin();
      while ( attIter != attr.attrs.end() )
      {
         if ( attIter->second.adaptorName == primaryKey.second )
         {
            primaryKey.first = attIter->first;
            break;
         }

         else
         {
            ++attIter;
         }
      }

      if ( attIter == attr.attrs.end() )
      {
         SAGA_THROW(std::string("Unknown attribute " + primaryKey.second),
                    saga::NoSuccess);
      }

      //Loop through this map until we find the adaptor version of the
      //attribute name in the data and then see what the key is
      attIter = relAttr.attrs.begin();
      while ( attIter != relAttr.attrs.end() )
      {
         if ( attIter->second.adaptorName == secondaryKey.second )
         {
            secondaryKey.first = attIter->first;
            break;
         }

         else
         {
            ++attIter;
         }
      }

      if ( attIter == relAttr.attrs.end() )
      {
         SAGA_THROW(std::string("Unknown attribute " + secondaryKey.second),
                    saga::NoSuccess);
      }
   }

   else
   {
      //A reverse lookup so primary is the related entity and
      //secondary is the current entity

      //Loop through this map until we find the adaptor version of the
      //attribute name in the data and then see what the key is
      attIter = relAttr.attrs.begin();
      while ( attIter != relAttr.attrs.end() )
      {
         if ( attIter->second.adaptorName == primaryKey.second )
         {
            primaryKey.first = attIter->first;
            break;
         }

         else
         {
            ++attIter;
         }
      }

      if ( attIter == relAttr.attrs.end() )
      {
         SAGA_THROW(std::string("Unknown attribute " + primaryKey.second),
                                saga::NoSuccess);
      }

      //Loop through this map until we find the adaptor version of the
      //attribute name in the data and then see what the key is
      attIter = attr.attrs.begin();
      while ( attIter != attr.attrs.end() )
      {
         if ( attIter->second.adaptorName == secondaryKey.second )
         {
            secondaryKey.first = attIter->first;
            break;
         }

         else
         {
            ++attIter;
         }
      }

      if ( attIter == attr.attrs.end() )
      {
         SAGA_THROW(std::string("Unknown attribute " + secondaryKey.second),
                    saga::NoSuccess);
      }
   }

   std::vector<saga::isn::entity_data> retEnt;

   retEnt = _ldap->get_related_entities(relatedEntityName,
                                        _model,
                                        _configDirectory,
                                        primaryKey,
                                        secondaryKey,
                                        reverse,
                                        relIter->second.directLookup,
                                        entities);

   //Now apply the filter to the entities
   std::vector<saga::isn::entity_data>::iterator iter = retEnt.begin();

   if ( filter.empty() == false )
   {
      bdii_query data_filter(_model, filter, relatedEntityName);

      std::vector<saga::isn::entity_data> filteredEntities;

      unsigned int count = 0;
      while ( iter != retEnt.end() )
      {
         bool selectEntity =
            data_filter.evaluate_data_filter(*iter);

         if ( selectEntity == true )
         {
            //add this element to the temp vector
            filteredEntities.push_back(*iter);
         }
         ++iter;
         ++count;
      }

      ret = filteredEntities;
      return;
   }

   else
   {
      ret = retEnt;
      return;
   }
   return;
}
}

