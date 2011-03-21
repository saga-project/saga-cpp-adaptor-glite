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
#include <map>
#include <vector>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

#include <ldap.h>

#include <boost/shared_ptr.hpp>

#include <saga/util.hpp>
#include <saga/url.hpp>
#include <saga/adaptors/task.hpp>
#include <saga/adaptors/attribute.hpp>

#include <impl/config.hpp>

#include "sd_bdii_query.hpp"
#include "bdii_provider.hpp"

#include "glite_discoverer.hpp"

#include "proxy_funcs.hpp"

#define SD_DEBUG 0

ANTLR_USING_NAMESPACE(antlr)

using namespace saga::sd;

namespace glite_sd_adaptor {

discoverer_cpi_impl::discoverer_cpi_impl (proxy		* p,
					  cpi_info       const & info,
                                          saga::ini::ini const & glob_ini,
                                          saga::ini::ini const & adap_ini,
                                          TR1::shared_ptr<saga::adaptor> adaptor)
   : base_cpi (p, info, adaptor, cpi::Noflags),
     _ini(adap_ini),
     _doGlue1(false),
     _doGlue2(false)
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

   //check the adaptor ini file to see if we're doing
   //GLUE1, GLUE2 or both
   if ( _ini.has_section("preferences") )
   {
      saga::ini::ini prefs = _ini.get_section ("preferences");

      if ( prefs.has_entry("glue1") )
      {
         std::string glue1 = prefs.get_entry("glue1");

         //Make the value lower case to aid our matching
         std::transform(glue1.begin(),
                        glue1.end(),
                        glue1.begin(),
                        static_cast<int(*)(int)>(std::tolower));

         if ( glue1 == "true" )
         {
            _doGlue1 = true;
         }
      }

      if ( prefs.has_entry("glue2") )
      {
         std::string glue2 = prefs.get_entry("glue2");

         //Make the value lower case to aid our matching
         std::transform(glue2.begin(),
                        glue2.end(),
                        glue2.begin(),
                        static_cast<int(*)(int)>(std::tolower));

         if ( glue2 == "true" )
         {
            _doGlue2 = true;
         }
      }
   }
}

discoverer_cpi_impl::~discoverer_cpi_impl (void)
{
}

void discoverer_cpi_impl::set_session(const boost::shared_ptr<saga::session> s)
{
   _session = s;
}

/* SAGA API functions */
/////////////////////////////////////////////////////////////////////////////////////
void discoverer_cpi_impl::sync_list_services2(
        std::vector<saga::sd::service_description>& ret, 
        std::string service_filter,
        std::string data_filter)
{
   std::string authz_filter;
   std::string authz_filter_1;
   std::string authz_filter_2;
   std::string identity;
   std::string errStr;
   std::vector<std::string> vo;
   std::vector<std::string> fqan;

   //Get the VOs and FQANs from the proxy certificate
   glite_adaptor::proxy_funcs& pf = glite_adaptor::proxy_funcs::Instance();

   bool res = pf.GetProxyAttributes(_proxy_location,
                                    identity,
                                    vo,
                                    fqan,
                                    errStr);

   if ( res != true )
   {
      SAGA_THROW(errStr, saga::NoSuccess);
   }

   if ( _doGlue1 )
   {
      authz_filter_1 = get_glue1_authz_filter(vo);
   }

   if ( _doGlue2 )
   {
      authz_filter_2 = get_glue2_authz_filter(vo, fqan, identity);
   }

   //Create the correct authz filter depending upon what
   //GLUE versions are in operation
   if ( _doGlue1 && !_doGlue2 )
   {
      authz_filter = authz_filter_1;
   }

   else if ( !_doGlue1 && _doGlue2 )
   {
      authz_filter = authz_filter_2;
   }

   else
   {
      authz_filter = authz_filter_1 + " or " + authz_filter_2;
   }

   //Now call list_services3 with our authz_filter
   sync_list_services3(ret, service_filter, data_filter, authz_filter);
}

std::string discoverer_cpi_impl::get_glue1_authz_filter(
   const std::vector<std::string>& vo) const
{
   std::string authz_filter = "(";
   std::vector<std::string>::size_type size = vo.size();

   if ( size > 0 )
   {
      std::vector<std::string>::const_iterator iter;
      std::vector<std::string>::const_iterator endIter = vo.end();

      //Iterate over all the entries in the VO vector
      for ( iter = vo.begin(); iter != endIter; ++iter )
      {
         //Each filter entry needs to be of the form
         //(VO = 'VO_NAME')
         authz_filter.append("(VO = '");

         //VO may well be in the form of VO://LOCATION:PORTNUM
         //so we have to deal with this
         //Look for the first ':'
         size_t pos = iter->find(':');

         //if ( pos != std::string::npos )
         //{
            authz_filter.append(iter->substr(0, pos));
         //}

         //else
         //{
         //   authz_filter.append(*iter);
         //}
         authz_filter.append("') or ");
      }

      //We also want services with no VO requirements
      authz_filter.append("(VO IS NULL)");

      //Close off our filter
      authz_filter.append(")");
   }

   else
   {
      //Nothing in the certificate so we only want services
      //with no VO requirements
      authz_filter.append("(VO IS NULL)");
   }
   return authz_filter;
}

std::string discoverer_cpi_impl::get_glue2_authz_filter(
   const std::vector<std::string>& vo, 
   const std::vector<std::string>& fqan,
   const std::string& identity) const
{
   std::string authz_filter;
   std::vector<std::string>::size_type size = vo.size() + fqan.size();

   if ( !identity.empty() )
   {
      ++size;
   }

   if ( size > 0 )
   {
      authz_filter = "(";

      std::vector<std::string>::const_iterator iter;
      std::vector<std::string>::const_iterator endIter = vo.end();

      //Iterate over all the entries in the VO vector
      for ( iter = vo.begin(); iter != endIter; ++iter )
      {
         //Each filter entry needs to be of the form
         //(VO = 'VO_NAME')
         authz_filter.append("(VO = '");

         //VO may well be in the form of VO://LOCATION:PORTNUM
         //so we have to deal with this
         //Look for the first ':'
         size_t pos = iter->find(':');

         //if ( pos != std::string::npos )
         //{
            authz_filter.append(iter->substr(0, pos));
         //}

         //else
         //{
         //   authz_filter.append(*iter);
         //}
         authz_filter.append("') or ");
      }

      endIter = fqan.end();

      //Iterate over all the entries in the FQAN vector
      for ( iter = fqan.begin(); iter != endIter; ++iter )
      {
         //Each filter entry needs to be of the form
         //(VO = 'VO_NAME')
         authz_filter.append("(FQAN = '");
         authz_filter.append(*iter);
         authz_filter.append("') or ");
      }

      //Add in our DN information
      if ( !identity.empty() )
      {
         authz_filter.append("(DN = '");
         authz_filter.append(identity);
         authz_filter.append("') or ");
      }

      //Add in matching for "ALL" in the GLUE2PolicyRule
      authz_filter.append("(ALL IS NOT NULL)");

      //Close off our filter
      authz_filter.append(")");
   }

   else
   {
      //Nothing in the certificate so we only want services
      //with no VO requirements
      authz_filter.append("(ALL IS NOT NULL");
   }
   return authz_filter;
}

void discoverer_cpi_impl::sync_list_services3(
        std::vector<saga::sd::service_description>& ret, 
        std::string service_filter,
        std::string data_filter,
        std::string vo_filter)
{
   instance_data data(this);
   std::string loc(data->location_.get_url());
   std::vector<saga::sd::service_description> sd_list;

   boost::shared_ptr<bdii_provider> bp(new bdii_provider(loc));
   bp->list_services(service_filter,
                     data_filter,
                     vo_filter,
                     ret,
                     _doGlue1,
                     _doGlue2);
#if SD_DEBUG
   std::cout << "list size in discoverer_cpi_impl = "
             << ret.size()
             << std::endl;
#endif

   /* set the information provider in service instances */
   /* set the session in service instances */
   for (unsigned int i = 0; i < ret.size(); i++)
   {
      saga::impl::info_provider::set_session(&(ret[i]), _session);
   }
}

} //Namespace

