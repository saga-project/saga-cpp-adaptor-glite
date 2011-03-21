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
#ifndef _LDAP_PROVIDER_HPP_
#define _LDAP_PROVIDER_HPP_

#include <string>
#include <vector>
#include <map>

#include <saga/saga/packages/isn/entity_data_set.hpp>
#include <saga/saga/packages/isn/entity_types.hpp>

#include "ldap.h"

class ldap_provider
{
   public:
      ldap_provider(const std::string& url);
      ~ldap_provider();

      std::vector<saga::isn::entity_data>
         get_entities(const std::string& model,
                      const std::vector<std::string>& filter,
                      const std::map<std::string,
                                     ENTITY_ATTR_MAP_TYPE>& attrs) const;

      std::vector<saga::isn::entity_data> get_related_entities(
         const std::string& relatedEntityName,
         const std::string& model,
         const std::string& configDirectory,
         const std::pair<std::string, std::string>& primaryKey,
         const std::pair<std::string, std::string>& secondaryKey,
         bool reverseLookup,
         bool directLookup,
         const std::vector<saga::isn::entity_data>& entities) const;

   private:
      ldap_provider();
      ldap_provider(const ldap_provider& other);
      ldap_provider& operator=(const ldap_provider& rhs);
      void ldap_connect();
      void ldap_disconnect();

      std::vector<std::string>
         get_attribute(LDAPMessage* entry, const std::string& ldapName) const;

      std::vector<std::string> get_related_entities_search_string(
         const std::string& relatedEntityName,
         const std::string& configDirectory,
         const std::pair<std::string, std::string>& primaryKey,
         const std::pair<std::string, std::string>& secondaryKey,
         bool reverseLookup,
         bool directLookup,
         const std::vector<saga::isn::entity_data>& entities) const;

      std::string _url;

      LDAP* _ld;
};



#endif

