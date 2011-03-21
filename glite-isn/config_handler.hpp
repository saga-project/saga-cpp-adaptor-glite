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
//config_handler.hpp
#ifndef _CONFIG_HANDLER_HPP_
#define _CONFIG_HANDLER_HPP_

#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <map>
#include <string>
#include <vector>

#include <saga/saga/packages/isn/entity_types.hpp>

//This class acts as our XML config file parser
//and also caches the information read in from said files
class config_handler : public XERCES_CPP_NAMESPACE_QUALIFIER DefaultHandler
{
public:
   config_handler();

   void startDocument();

   void endDocument();
    
   void startElement(const XMLCh* const uri, 
                     const XMLCh* const localname, 
                     const XMLCh* const qname, 
                     const XERCES_CPP_NAMESPACE_QUALIFIER Attributes& attrs);
    
   void endElement(const XMLCh* const uri, 
                   const XMLCh* const localname,
                   const XMLCh* const qname);

   void characters(const XMLCh* const chars,
                   const unsigned int length);

   static bool is_entity_known(const std::string& entityKey) throw();

   static std::vector<std::string>
      get_entity_attributes(const std::string& entityType,
                            const std::string& configDirectory);

   static ENTITY_ATTR_TYPE get_entity(const std::string& entityType,
                                      const std::string& configDirectory);
   static ENTITY_ATTR_TYPE get_known_entity(const std::string& entityKey);

private:
   config_handler(const config_handler& other);
   config_handler operator= (const config_handler& other);

   void ProcessEntity
      (const XERCES_CPP_NAMESPACE_QUALIFIER Attributes& attrs);

   void ProcessRelatedEntity
      (const XERCES_CPP_NAMESPACE_QUALIFIER Attributes& attrs);

   void ProcessRelationship
      (const XERCES_CPP_NAMESPACE_QUALIFIER Attributes& attrs);
   
   void ProcessAttribute
      (const XERCES_CPP_NAMESPACE_QUALIFIER Attributes& attrs);

   std::string _name;
   std::string _ldapName;
   std::map<std::string, ENTITY_ATTR_MAP_TYPE> _attrMap;
   std::multimap<std::string, ENTITY_RELATIONSHIP_TYPE> _relatedEntities;

   //Static map of entity name to its attributes
   static std::map<std::string, ENTITY_ATTR_TYPE> _knownEntities;

   std::string _curRelatedEntity;
   std::string _curRelatedEntityLdap;
};

#endif

