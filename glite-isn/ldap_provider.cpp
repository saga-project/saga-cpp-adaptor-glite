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
#include <ldap.h>

#include <iostream>
#include <sstream>
#include <set>

#include <saga/adaptors/adaptor.hpp>

#include "ldap_provider.hpp"
#include "config_handler.hpp"

ldap_provider::ldap_provider(const std::string& url)
{
   _url = url;
   _ld = NULL;

   //Connect to LDAP server
   ldap_connect();
}

ldap_provider::~ldap_provider()
{
   ldap_disconnect();
}

void ldap_provider::ldap_connect()
{
   //Check that we're not already connected to something
   if ( _ld != NULL )
   {
      ldap_disconnect();
   }

   //STEP 1: LDAP Init
   int rc = ldap_initialize(&_ld, _url.c_str());

   if ( rc != LDAP_SUCCESS )
   {
      _ld = NULL;
      std::ostringstream errStr;
      errStr << "ldap_initialize: " << ldap_err2string(rc);
      SAGA_THROW_NO_OBJECT(std::string(errStr.str()),
                           saga::NoSuccess);
   }

   //Use the LDAP_OPT_PROTOCOL_VERSION session preference to specify
   //that the client is an LDAPv3 client.
   int version = LDAP_VERSION3;
   ldap_set_option(_ld, LDAP_OPT_PROTOCOL_VERSION, &version);

   //STEP 2: Bind to the server.  The client binds anonymously to the server
   //(no DN or credentials are specified).
   rc = ldap_simple_bind_s(_ld, "", "");

   if ( rc != LDAP_SUCCESS )
   {
      std::ostringstream errStr;
      errStr << "ldap_simple_bind: " << ldap_err2string(rc);
      SAGA_THROW_NO_OBJECT(std::string(errStr.str()),
                           saga::NoSuccess);
   }
}

void ldap_provider::ldap_disconnect()
{
   if ( _ld != NULL )
   {
      ldap_unbind_ext(_ld, NULL, NULL);
   }
   _ld = NULL;
}

std::vector<std::string>
   ldap_provider::get_attribute(LDAPMessage* entry,
                                const std::string& ldapName) const
{
   char** values;
   char** valPtr;

   std::vector<std::string> retVal;

   values = ldap_get_values(_ld, entry, ldapName.c_str());
   valPtr = values;

   //Did we get any returned value?
   while ( (valPtr != NULL) &&
           (*valPtr != NULL) )
   {
      retVal.push_back(std::string(*valPtr));

      //Next result
      ++valPtr;
   }
   //Free allocated memory
   ldap_value_free(values);

   return retVal;
}

std::vector<saga::isn::entity_data> ldap_provider::
   get_entities(const std::string& model,
                const std::vector<std::string>& filters,
                const std::map<std::string,
                               ENTITY_ATTR_MAP_TYPE>& attrs) const
{
   //Set up our timeout
   struct timeval timeout;
   timeout.tv_sec = 60;
   timeout.tv_usec = 0;

   //We need to create an array of char* to hold the attributes
   //in that we're after
   //First get the count
   unsigned int attrCount = attrs.size();

   const char** glueAttrs = new const char* [attrCount + 1];

   std::map<std::string, ENTITY_ATTR_MAP_TYPE>::const_iterator iter;
   std::map<std::string,
            ENTITY_ATTR_MAP_TYPE>::const_iterator endIter = attrs.end();
   unsigned int count = 0;

   for ( iter = attrs.begin(), count = 0;
         iter != endIter;
         ++iter, ++count )
   {
      glueAttrs[count] = iter->second.adaptorName.c_str();
   }

   //Remember to finish array with a NULL
   //Remember arrays are 0 based
   glueAttrs[attrCount] = NULL;

   //Loop over every element in our filter vector
   std::vector<std::string>::const_iterator vecIter = filters.begin();

   std::vector<saga::isn::entity_data> retVal;

   while ( vecIter != filters.end() )
   {
      std::string filter = *vecIter;
      ++vecIter;
      LDAPMessage* searchResult = NULL;

      std::string ldapBase;

      if ( model == "glue1" )
      {
         ldapBase = "o=grid";
      }

      else if ( model == "glue2" )
      {
         ldapBase = "o=glue";
      }

      else
      {
         SAGA_THROW_NO_OBJECT("Unknown model: " + model,
                              saga::BadParameter);
      }

      int rc = ldap_search_st(_ld,
                              ldapBase.c_str(),
                              LDAP_SCOPE_SUBTREE,
                              filter.c_str(),
                              (char**)glueAttrs,
                              0,
                              &timeout,
                              &searchResult);

      //Free up allocated resources
      delete [] glueAttrs;
      glueAttrs = NULL;
      
      if ( rc != LDAP_SUCCESS )
      {
         std::ostringstream errStr;
         errStr << "ldap_search_st: " << ldap_err2string(rc);
         SAGA_THROW_NO_OBJECT(std::string(errStr.str()),
                              saga::NoSuccess);
      }

      LDAPMessage* entry;

      for ( entry = ldap_first_entry(_ld, searchResult);
            entry != NULL;
            entry = ldap_next_entry(_ld, entry))
      {
         //Create an entity to hold the results in
         saga::isn::entity_data entity;

         //Loop through all the glue attributes
         //and set them in the entity
         for ( iter = attrs.begin(); iter != endIter; ++iter )
         {
            std::vector<std::string> attrVec;
            attrVec = get_attribute(entry, iter->second.adaptorName);

            //If this is a vector attribute then add all the values
            //otherwise just add the first

            if ( iter->second.multivalued == true )
            {
               entity.set_vector_attribute(iter->first, attrVec);
            }

            else if ( attrVec.size() > 0 )
            {
               entity.set_attribute(iter->first, attrVec.at(0));
            }
         }
         //Add this result to our return value
         retVal.push_back(entity);
      }

      ldap_msgfree(searchResult);
      searchResult= NULL;
   }

   return retVal;
}

std::vector<saga::isn::entity_data> ldap_provider::
   get_related_entities(const std::string& relatedEntityName,
                        const std::string& model,
                        const std::string& configDirectory,
                        const std::pair<std::string,
                                        std::string>& primaryKey,
                        const std::pair<std::string,
                                        std::string>& secondaryKey,
                        bool reverseLookup,
                        bool directLookup,
                        const std::vector<saga::isn::entity_data>&
                           entities) const
{
   std::vector<std::string> query =
      get_related_entities_search_string(relatedEntityName,
                                         configDirectory,
                                         primaryKey,
                                         secondaryKey,
                                         reverseLookup,
                                         directLookup,
                                         entities);

   return get_entities(model,
                       query,
                       config_handler::get_entity(relatedEntityName,
                                                  configDirectory).attrs);
}

std::vector<std::string> ldap_provider::get_related_entities_search_string(
   const std::string& relatedEntityName,
   const std::string& configDirectory,
   const std::pair<std::string, std::string>& primaryKey,
   const std::pair<std::string, std::string>& secondaryKey,
   bool reverseLookup,
   bool directLookup,
   const std::vector<saga::isn::entity_data>& entities) const
{
   std::vector<std::string> retVal;

   //Start constructing the LDAP search string
   ENTITY_ATTR_TYPE attr = config_handler::get_entity(relatedEntityName,
                                                      configDirectory);
   std::string ldapQuery = "(&(objectClass=" + attr.adaptorName + ")";

   std::vector<saga::isn::entity_data>::const_iterator iter;
   std::vector<saga::isn::entity_data>::const_iterator endIter =
      entities.end();

   //Set of strings to keep our individual queries in
   //We use a set to remove duplicates
   std::set<std::string> searchStrings;

   //Loop through all the entities and add them to the query
   for ( iter = entities.begin(); iter != endIter; ++iter )
   {
      //Deal with forward and reverse relationships
      if ( reverseLookup == true )
      {
         std::vector<std::string> vals;

         if ( iter->attribute_is_vector(secondaryKey.first) )
         {
            vals = iter->get_vector_attribute(secondaryKey.first);
         }

         else
         {
            vals.push_back(iter->get_attribute(secondaryKey.first));
         }

         std::vector<std::string>::const_iterator valIter;
         std::vector<std::string>::const_iterator valEndIter = vals.end();

         //Loop through every matching attribute in the entity
         for ( valIter = vals.begin(); valIter != valEndIter; ++valIter )
         {
            std::string searchString;
            if ( directLookup == false )
            {
               searchString += "(" + primaryKey.second +
                               "=" + secondaryKey.second +
                               "=" + *valIter +
                               ")";
            }

            else
            {
               searchString += "(" + primaryKey.second +
                               "=" + *valIter +
                               ")";
            }

            //Add this searchString to our larger set
            searchStrings.insert(searchString);
         }
      }

      else
      {
         std::vector<std::string> vals;

         if ( iter->attribute_is_vector(primaryKey.first) )
         {
            vals = iter->get_vector_attribute(primaryKey.first);
         }

         else
         {
            vals.push_back(iter->get_attribute(primaryKey.first));
         }

         std::vector<std::string>::const_iterator valIter;
         std::vector<std::string>::const_iterator valEndIter = vals.end();

         //Loop through every matching attribute in the entity
         for ( valIter = vals.begin(); valIter != valEndIter; ++valIter )
         {
            if ( directLookup == false )
            {
               //We need to see if the key we're interested in
               //is in this entity's attribute
               std::string lookupString = secondaryKey.second + "=";

               //We need to do case insensite searching as
               //the information provider is a bit flakey in this respect
               std::transform(lookupString.begin(),
                              lookupString.end(),
                              lookupString.begin(),
                              ::toupper);

               std::string UCattrString = *valIter;
               std::transform(UCattrString.begin(),
                              UCattrString.end(),
                              UCattrString.begin(),
                              ::toupper);

               std::string::size_type loc =
                  UCattrString.find(lookupString);

               //Did we find what we were looking for?
               if ( loc != std::string::npos )
               {
                  //Add on the length of the search string
                  //as we don't want it
                  loc += lookupString.length();

                  //string is of the form
                  //SomeAttr=(Something=whatever,)?WhatWeWant=value(,...)*
                  std::string::size_type endLoc;
                  endLoc = valIter->find_first_of(',', loc);

                  std::string value(*valIter, loc, endLoc - loc);

                  std::string searchString;

                  searchString = "(" +
                                 secondaryKey.second +
                                 "=" +
                                 value + ")";

                  //Add this searchString to our larger set
                  searchStrings.insert(searchString);
               }
            }

            else
            {
               std::string searchString = "(" +
                                          secondaryKey.second +
                                          "=" +
                                          *valIter + ")";

               //Add this searchString to our larger set
               searchStrings.insert(searchString);
            }
         }
      }
   }

   //Check that we have something to search for
   if ( searchStrings.size() == 0 )
   {
      return retVal;
   }

   std::string curQuery;
   //Reserve some space for our long query.
   //This is a little longer than the max query length we check for
   //to avoid any extra memory allocations
   curQuery.reserve(11000);

   //Loop over every element in our query set and add it to a longer
   //query.  Stop if the query gets too long and start again.
   std::set<std::string>::const_iterator setIter = searchStrings.begin();

   bool stringAdded = false;

   while ( setIter != searchStrings.end() )
   {
      //If we have more than one entity in this set we need to add
      //an OR into the query
      if ( stringAdded == false )
      {
         curQuery = ldapQuery + "(|";
         stringAdded = true;
      }

      curQuery += *setIter;

      //Increment our iterator
      ++setIter;

      //close off this query if it's too long or we've finished
      if ( (curQuery.length() > 10000) ||
           (setIter == searchStrings.end()) )
      {
         //Close the query
         curQuery += "))";

         //Add this query to our vector of queries
         retVal.push_back(curQuery);

         //Reset our flag
         stringAdded = false;
      }
   }
   return retVal;
}

