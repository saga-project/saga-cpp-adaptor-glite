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
#include "config_handler.hpp"
#include <iostream>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>

#include <saga/adaptors/adaptor.hpp>

#include "helper.hpp"

#include <utility>
#include <cassert>

#ifdef XERCES_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE
#endif

////Constants for the "entity" tag and its attribute
const std::string k_entity = "entity";
const std::string k_entityName = "name";
const std::string k_entityLdapName = "ldapname";

//Constants for the "relatedEntity" tag and its attributes
const std::string k_relatedEntity = "relatedEntity";
const std::string k_relatedEntityName = "name";
const std::string k_relatedEntityLdapName = "ldapname";

const std::string k_relationship = "relationship";
const std::string k_relationshipPrimaryKey = "primaryKey";
const std::string k_relationshipSecondaryKey = "secondaryKey";
const std::string k_relationshipReverseLookup = "reverseLookup";
const std::string k_relationshipDirectLookup = "directLookup";

//Constants for the "attribute" tag and its attributes
const std::string k_attribute = "attribute";
const std::string k_attrName = "name";
const std::string k_attrLdapName = "ldapname";
const std::string k_attrMultivalued = "multivalued";
const std::string k_attrType = "type";

std::string model;

std::map<std::string, ENTITY_ATTR_TYPE> config_handler::_knownEntities;

//Public constructor
config_handler::config_handler()
{
}

//Static helper funtion to return a vector
//of the attribute names for an entity
std::vector<std::string>
   config_handler::get_entity_attributes(const std::string& entityType,
                                         const std::string& configDirectory)
{
   //Get the requested entity
   ENTITY_ATTR_TYPE attr = get_entity(entityType, configDirectory);

   std::vector<std::string> retVal;

   //Loop through the attributes for this entity type
   std::map<std::string, ENTITY_ATTR_MAP_TYPE>::const_iterator iter;
   std::map<std::string, ENTITY_ATTR_MAP_TYPE>::const_iterator endIter =
      attr.attrs.end();

   for ( iter = attr.attrs.begin(); iter != endIter; ++iter )
   {
      //Add attribute to the vector
      retVal.push_back(iter->second.adaptorName);
   }

   return retVal;
}

//Static helper function to determine whether or not
//we've previously parsed the XML file for this type of entity
bool config_handler::is_entity_known(const std::string& entityKey) throw()
{
   //Look for the given entityType
   if ( _knownEntities.find(entityKey) == _knownEntities.end() )
   {
      return false;
   }

   else
   {
      return true;
   }
}

//Static helper function to return the cached details for a known entity type
ENTITY_ATTR_TYPE
   config_handler::get_known_entity(const std::string& entityKey)
{
   //Do we know about this entityType?
   if ( is_entity_known(entityKey) == false )
   {
      SAGA_THROW_NO_OBJECT(std::string("Unknown entity: " + entityKey),
                           saga::BadParameter);
   }

   return _knownEntities.find(entityKey)->second;
}


//Static helper function to return the details for an entity type
//Will check for cached details otherwise it will load the relevant
//XML file and parse it
ENTITY_ATTR_TYPE config_handler::get_entity(const std::string& entityType,
                                            const std::string& configDirectory)
{
   ENTITY_ATTR_TYPE attr;
   model = configDirectory;
   std::string entityKey = model + entityType;

   //Do we need to read in an XML file?
   if ( is_entity_known(entityKey) )
   {
      attr =  get_known_entity(entityKey);
   }

   else
   {
      //Initialize the XML library
      XMLPlatformUtils::Initialize();

      //Dummy scope for the auto_ptr
      {
         std::auto_ptr<SAX2XMLReader>
            pParser(XMLReaderFactory::createXMLReader());

         //Set up how we parse XML files
         pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
         pParser->setFeature(XMLUni::fgXercesDynamic, true);

         //Set our specific handler
         config_handler handler;

         pParser->setContentHandler(&handler);
         pParser->setErrorHandler(&handler);

         //Load the XML file
         std::string filename = configDirectory + entityType + ".xml";

         //Now parse the xml file using SAX2
         try
         {
            pParser->parse(filename.c_str());
         }

         catch (...)
         {
            SAGA_THROW_NO_OBJECT("Invalid entity name: " + entityType,
                                 saga::BadParameter);
         }
      }

      //Terminate the XML library
      XMLPlatformUtils::Terminate();

      //Double check
      assert(is_entity_known(entityKey));

      attr = get_known_entity(entityKey);
   }
   return attr;
}

//Put any start of document specific processing in here
void config_handler::startDocument()
{
   //std::cout << "Starting..." << std::endl;
}

//Put any end of document specific processing in here
//In this case we populate the static _knownEntities container
//with the the name and details for the "just processed" entity
void config_handler::endDocument()
{
   //std::cout << "Ending..." << std::endl;

   //Add this data to our static map
   ENTITY_ATTR_TYPE attr;
   attr.adaptorName = _ldapName;
   attr.attrs = _attrMap;
   attr.relatedEntities = _relatedEntities;

   _knownEntities.insert(std::make_pair(model + _name, attr));
}

//Put any specific start of element processing in here
void config_handler::startElement(const XMLCh* const uri, 
                                  const XMLCh* const localname, 
                                  const XMLCh* const qname, 
                                  const Attributes& attrs)
{
   try
   {
      StrX uri_str(uri);
      StrX ln_str(localname);
      StrX qn_str(qname);

      /*std::cout << "Starting element... ["
                << uri_str
                << " "
                << ln_str
                << " "
                << qn_str
                << "]"
                << std::endl;*/

      //Is this an entity Element?
      if ( k_entity == ln_str.localForm() )
      { 
         //Process "Entity" tag
         ProcessEntity(attrs);
      }

      //Is this a relatedEntity Element?
      else if ( k_relatedEntity == ln_str.localForm() )
      { 
         //Process "Related Entity" tag
         ProcessRelatedEntity(attrs);
      }

      else if ( k_attribute == ln_str.localForm() )
      {
         //Process "Attribute" tag
         ProcessAttribute(attrs);
      }

      else if ( k_relationship == ln_str.localForm() )
      {
         //Process "relationship" tag
         ProcessRelationship(attrs);
      }
   }

   catch (const XMLException& toCatch)
   {
      std::ostringstream output;
      output << "An error occurred.  Error: "
                << StrX(toCatch.getMessage());

      SAGA_THROW_NO_OBJECT(output.str(), saga::NoSuccess);
   }

   catch (...)
   {
      SAGA_THROW_NO_OBJECT("An unknown error occurred.", saga::NoSuccess);
   }
}

//Put any specific end of element processing in here
void config_handler::endElement(const XMLCh* const uri, 
                                const XMLCh* const localname,
                                const XMLCh* const qname)
{
   //std::cout << "Ending element..." << std::endl << std::endl;

   try
   {
      StrX ln_str(localname);

      //If we've just finished a "relatedEntity" tag then unset our
      //private copy of the name
      if ( k_relatedEntity == ln_str.localForm() )
      { 
         _curRelatedEntity = "";
         _curRelatedEntityLdap = "";
      }
   }

   catch (const XMLException& toCatch)
   {
      std::ostringstream output;
      output << "An error occurred.  Error: "
                << StrX(toCatch.getMessage());

      SAGA_THROW_NO_OBJECT(output.str(), saga::NoSuccess);
   }

   catch (...)
   {
      SAGA_THROW_NO_OBJECT("An unknown error occurred.", saga::NoSuccess);
   }
}

void config_handler::characters(const XMLCh* const chars,
                                const unsigned int length)
{
   StrX strChars = chars;
   /*std::cout << length
             << "chars ["
             << strChars.localForm()
             << "]"
             << std::endl;*/
}

//Process an entity tag in an XML file
void config_handler::ProcessEntity(const Attributes& attrs)
{
   XMLSize_t numAttr = attrs.getLength();

   //Loop through all the attributes
   for ( XMLSize_t curAttr = 0; curAttr < numAttr; ++curAttr )
   {
      StrX ln = attrs.getLocalName(curAttr);
      StrX att = attrs.getValue(curAttr);

      //Get the "name" entry
      if ( k_entityName == ln.localForm() )
      {
         _name = att.localForm();
      }

      //Get the "ldapname" entry
      else if ( k_entityLdapName == ln.localForm() )
      {
         _ldapName = att.localForm();
      }
   }
}

//Process a relatedEntity tag in an XML file
void config_handler::ProcessRelatedEntity(const Attributes& attrs)
{
   XMLSize_t numAttr = attrs.getLength();

   //Loop through all the attributes
   for ( XMLSize_t curAttr = 0; curAttr < numAttr; ++curAttr )
   {
      StrX ln = attrs.getLocalName(curAttr);
      StrX att = attrs.getValue(curAttr);

      //Get the "name" entry
      if ( k_relatedEntityName == ln.localForm() )
      {
         //Store this in our private data so the "relationship" tag
         //handler knows what it's working on
         _curRelatedEntity = att.localForm();
      }

      //Get the "name" entry
      else if ( k_relatedEntityLdapName == ln.localForm() )
      {
         //Store this in our private data so the "relationship" tag
         //handler knows what it's working on
         _curRelatedEntityLdap = att.localForm();
      }
   }
}

//Process a relationship tag in an XML file
void config_handler::ProcessRelationship(const Attributes& attrs)
{
   XMLSize_t numAttr = attrs.getLength();

   ENTITY_RELATIONSHIP_TYPE rel;
   rel.reverseLookup = false;
   rel.directLookup = false;

   //Loop through all the attributes
   for ( XMLSize_t curAttr = 0; curAttr < numAttr; ++curAttr )
   {
      StrX ln = attrs.getLocalName(curAttr);
      StrX att = attrs.getValue(curAttr);

      //Get the "primaryKey" entry
      if ( k_relationshipPrimaryKey == ln.localForm() )
      {
         rel.primaryKey = att.localForm();
      }

      //Get the "secondaryKey" entry
      else if ( k_relationshipSecondaryKey == ln.localForm() )
      {
         rel.secondaryKey = att.localForm();
      }

      //Get the "reverseLookup" entry
      else if ( k_relationshipReverseLookup == ln.localForm() )
      {
         rel.reverseLookup = att.localForm();
      }

      //Get the "directLookup" entry
      else if ( k_relationshipDirectLookup == ln.localForm() )
      {
         rel.directLookup = att.localForm();
      }
   }

   //Add this entry to our multimap
   if ( (! _curRelatedEntity.empty()) &&
        (! _curRelatedEntityLdap.empty()) )
   {
      rel.entityLdapName = _curRelatedEntityLdap;
      _relatedEntities.insert(std::make_pair(_curRelatedEntity, rel));
   }
}

//Process an attribute tag in an XML file
void config_handler::ProcessAttribute(const Attributes& attrs)
{
   XMLSize_t numAttr = attrs.getLength();

   std::string glueName;
   ENTITY_ATTR_MAP_TYPE attr;
   attr.multivalued = false;

   //Loop through all the attributes
   for ( XMLSize_t curAttr = 0; curAttr < numAttr; ++curAttr )
   {
      StrX ln = attrs.getLocalName(curAttr);
      StrX att = attrs.getValue(curAttr);

      /*std::cout << "Attribute : "
       *          << ln
       *          << " = "
       *          << att
       *          << std::endl;
       */

      //Get the Glue name
      if ( k_attrName == ln.localForm() )
      {
         glueName = att.localForm();
      }

      //Get the LDAP version of the Glue name
      else if ( k_attrLdapName == ln.localForm() )
      {
         attr.adaptorName = att.localForm();
      }

      //Get the multivalued flag
      else if ( k_attrMultivalued == ln.localForm() )
      {
         //The value is already false so we only need
         //to deal with the true cases
         if ( (std::string("true") == att.localForm()) ||
              (std::string("True") == att.localForm()) ||
              (std::string("TRUE") == att.localForm()) )
         {
            attr.multivalued = true;
         }
      }

      //Get the type of the attribute
      else if ( k_attrType== ln.localForm() )
      {
         attr.type = att.localForm();
      }
   }

   //Check that there was a "name" field 
   //if so add this struct to our map
   if ( ! glueName.empty() )
   {
      _attrMap.insert(std::make_pair(glueName, attr));
   }
}

