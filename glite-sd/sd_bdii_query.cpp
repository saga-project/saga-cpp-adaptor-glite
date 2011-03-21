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
#include "sd_bdii_query.hpp"
#include "sdTokenTypes.hpp"

#include <boost/regex.hpp>

#include <sstream>

//Non-exported helper functions
//Don't need to be a part of the class but are useful here
namespace
{
   std::string CreateRegexString(const std::string& filter,
                                 const char escapeChar)
   {
      std::string::size_type len = filter.size();
      std::string::size_type curPos;

      //Start regex with '^' for beginning of string
      std::string regexStr = "^";
      char curChar;

      //Loop through our input string one char at a time
      for ( curPos = 0; curPos < len; ++curPos )
      {
         curChar = filter[curPos];

         //Deal with the SQL LIKE statement escape char
         //A switch would be nicer but can't test against a variable :(
         if ( curChar == escapeChar )
         {
            //Is this character escaping anything?
            if ( curPos < (len - 1) )
            {
               char nextChar = filter[curPos + 1];

               //Yes, so ignore this character and
               //use the next one without replacing it with its
               //regex equivalent
               if ( (nextChar == '%') ||
                    (nextChar == '_') )
               {
                  regexStr += nextChar;
                  ++curPos;
               }

               //No, so just pass it through
               else
               {
                  regexStr += curChar;
               }
            }

            //Last character so just pass it through
            else
            {
               regexStr += curChar;
            }
         }

         //Escape characters that are special to our regex
         else if ( (curChar == '.') ||
                   (curChar == '*') ||
                   (curChar == '[') ||
                   (curChar == ']') ||
                   /*(curChar == '^') ||
                   (curChar == '$') ||*/
                   (curChar == '\\') )
         {
            regexStr += '\\';
            regexStr += curChar;
         }

         //Change % to .* for wildcat matching
         else if ( curChar == '%' )
         {
            regexStr += ".*";
         }

         //Change _ to . for simgle character matching
         else if ( curChar == '_' )
         {
            regexStr += '.';
         }

         //Just pass all other characters through
         else
         {
            regexStr += curChar;
         }
      }
      //End regex with '$' for end of string
      regexStr += '$';

      return regexStr;
   }

   std::string ConvertServiceString(const std::string& filter,
                                    const char escapeChar)
   {
      std::string::size_type len = filter.size();
      std::string::size_type curPos;

      std::string outStr;
      char curChar;

      //Loop through our input string one char at a time
      for ( curPos = 0; curPos < len; ++curPos )
      {
         curChar = filter[curPos];

         //Deal with the SQL LIKE statement escape char
         //A switch would be nicer but can't test against a variable :(
         if ( curChar == escapeChar )
         {
            //Is this character escaping anything?
            if ( curPos < (len - 1) )
            {
               char nextChar = filter[curPos + 1];

               //Yes, so ignore this character and
               //use the next one without replacing it with its
               //regex equivalent
               if ( (nextChar == '%') ||
                    (nextChar == '_') )
               {
                  outStr += nextChar;
                  ++curPos;
               }

               //No, so just pass it through
               else
               {
                  outStr += curChar;
               }
            }

            //Last character so just pass it through
            else
            {
               outStr += curChar;
            }
         }

         //Escape characters that are special to our search
         else if ( curChar == '*' )
         {
            outStr += "\\2a";
            outStr += curChar;
         }

         //Change '%' and '_' to '*' for wildcat matching
         //We can't easily do single character wildcats so
         //we use the more general '*'
         else if ( (curChar == '%') ||
                   (curChar == '_') )
         {
            outStr += "*";
         }

         //Just pass all other characters through
         else
         {
            outStr += curChar;
         }
      }

      return outStr;
   }

   void RemoveOuterQuotes(std::string& inStr)
   {
      std::string::size_type len = inStr.size();

      //Check we have at least 2 characters
      if ( len > 1 )
      {
         //If first and last characters are single quotes
         //then remove them
         if ( (inStr[0] == '\'') &&
              (inStr[len - 1] == '\'') )
         {
            inStr = inStr.substr(1, len - 2);
         }
      }
   }
}

/* PRIVATE FUNCTIONS */
std::string
   sd_bdii_query::get_capability_string(const std::string& capability) const
{
   //Get ldap attribute name
   std::string attr = get_svc_keyword(sdTokenTypes::K_CAPABILITY);

   //Get all the values for this capability
   std::pair<CAP_CITER, CAP_CITER> iter_range;

   iter_range = capability_map.equal_range(capability);

   CAP_CITER iter;

   if ( iter_range.first == iter_range.second )
   {
      SAGA_THROW("Invalid query.  Unknown Capability '" +
                 capability +
                 "'",
                 saga::BadParameter);
   }

   //ostringstream for creating our output string
   std::ostringstream outStr;

   //Loop over them all
   for ( iter = iter_range.first; iter != iter_range.second; ++iter )
   {
      outStr << "(" << attr << "=" << iter->second << ")";
   }

   return outStr.str();
}

string sd_bdii_query::get_svc_keyword(int ktoken) const
{
   string kword;

   if ( _glueVersion == 1 )
   {
      switch (ktoken)
      {
         case sdTokenTypes::K_TYPE:
            kword = "GlueServiceType";
            break;
         case sdTokenTypes::K_NAME:
            kword = "GlueServiceName";
            break;
         case sdTokenTypes::K_UID:
            kword = "GlueServiceUniqueID";
            break;
         case sdTokenTypes::K_URI:
            kword = "GlueServiceEndpoint";
            break;
         case sdTokenTypes::K_SITE:
            kword = "GlueForeignKey=GlueSiteUniqueID";
            break;
         case sdTokenTypes::K_ENDPOINT:
            kword = "GlueServiceEndpoint";
            break;
         case sdTokenTypes::K_SERVICE:
            kword = "GlueForeignKey=GlueServiceUniqueID";
            break;
         case sdTokenTypes::K_VO:
         case sdTokenTypes::K_VOMS:
         case sdTokenTypes::K_FQAN:
         case sdTokenTypes::K_DN:
            kword = "GlueServiceOwner";
            break;
         case sdTokenTypes::K_ALL:
            kword = "all";
            break;
         case sdTokenTypes::K_INTERFACE_VERSION:
            kword = "GlueServiceVersion";
            break;
         case sdTokenTypes::K_IMPLEMENTATION_VERSION:
            kword = "GlueServiceVersion";
            break;
         case sdTokenTypes::K_CAPABILITY:
            kword = "GlueServiceType";
            break;
         default:
            SAGA_THROW_NO_OBJECT("Unknown filter token",
                                 saga::BadParameter);
      }
   }

   else if ( _glueVersion == 2 )
   {
      switch (ktoken)
      {
         case sdTokenTypes::K_TYPE:
            kword = "GLUE2ServiceType";
            break;
         case sdTokenTypes::K_NAME:
            kword = "GlueServiceName";
            break;
         case sdTokenTypes::K_UID:
            kword = "GLUE2ServiceID";
            break;
         case sdTokenTypes::K_URI:
            kword = "GlueServiceEndpoint";
            break;
         case sdTokenTypes::K_SITE:
            kword = "GlueForeignKey=GlueSiteUniqueID";
            break;
         case sdTokenTypes::K_ENDPOINT:
            kword = "GlueServiceEndpoint";
            break;
         case sdTokenTypes::K_SERVICE:
            kword = "GlueForeignKey=GlueServiceUniqueID";
            break;
         case sdTokenTypes::K_VO:
            kword = "GlueServiceOwner";
            break;
         case sdTokenTypes::K_INTERFACE_VERSION:
            kword = "GlueServiceVersion";
            break;
         case sdTokenTypes::K_IMPLEMENTATION_VERSION:
            kword = "GlueServiceVersion";
            break;
         case sdTokenTypes::K_CAPABILITY:
            kword = "GLUE2ServiceCapability";
            break;
         default:
            SAGA_THROW_NO_OBJECT("Unknown filter token",
                                 saga::BadParameter);
      }
   }

   else
   {
      SAGA_THROW("Invalid query.  Unknown GLUE version",
                 saga::BadParameter);
   }
   return kword;
}

std::vector<std::string>
   sd_bdii_query::get_service_values(const saga::sd::service_description& sd,
                                  int ktoken)
{
   std::string key;
   std::string value;
   std::vector<std::string> retVal;

   switch (ktoken)
   {
      case sdTokenTypes::K_TYPE:
         key = saga::sd::attributes::service_description_type;
         break;
      case sdTokenTypes::K_NAME:
         key = saga::sd::attributes::service_description_name;
         break;
      case sdTokenTypes::K_UID:
         key = saga::sd::attributes::service_description_uid;
         break;
      case sdTokenTypes::K_URI:
         key = saga::sd::attributes::service_description_url;
         break;
      case sdTokenTypes::K_SITE:
         key = saga::sd::attributes::service_description_site;
         break;
      case sdTokenTypes::K_ENDPOINT:
         key = saga::sd::attributes::service_description_url;
         break;
      case sdTokenTypes::K_SERVICE:
         key = saga::sd::attributes::service_description_relatedservices;
         break;
      case sdTokenTypes::K_INTERFACE_VERSION:
         key = saga::sd::attributes::service_description_interface_version;
         break;
      case sdTokenTypes::K_IMPLEMENTATION_VERSION:
         key = saga::sd::attributes::service_description_implementation_version;
         break;
      case sdTokenTypes::K_IMPLEMENTOR:
         key = saga::sd::attributes::service_description_implementor;
         break;
      case sdTokenTypes::K_CAPABILITY:
         key = saga::sd::attributes::service_description_capability;
         break;
      default:
         SAGA_THROW_NO_OBJECT("Unknown service token", saga::BadParameter);
   }

   //Check attribute exists
   if ( sd.attribute_exists(key) )
   {
      if ( sd.attribute_is_vector(key) )
      {
         retVal = sd.get_vector_attribute(key);
      }

      else
      {
         retVal.push_back(sd.get_attribute(key));
      }
   }

   else
   {
      std::string err("Service attribute does not exist: " + key);
      SAGA_THROW_NO_OBJECT(err, saga::BadParameter);
   }

   return retVal;
}

void sd_bdii_query::find_and_replace(string& source,
                                  const string& fstr,
                                  const string& rstr)
{
   std::string::size_type i = 0;

   while ( (i = source.find(fstr, i)) != std::string::npos )
   {
      if ( i != std::string::npos )
      {
         //If found replace it
         source.replace(i, fstr.length(), rstr);

         //Now move on to the next character after the replacement string
         i += rstr.length();
      }
   }
}


void sd_bdii_query::eval_service_filter(RefAST top,
                                     saga::sd::service_description& sd)
{
   int tok_type;

   if (top != NULL)
   {
      tok_type = top->getType();
      //cout << "Token type:" << tok_type << endl;

      // LEAF NODE
      if (tok_type == sdTokenTypes::SVC_CMP)
      {
         Refsd_leafnode nodeRef;
         nodeRef = (Refsd_leafnode)top;
         bool result = false;

         int lkey = nodeRef->getLValue();
         int op_type = nodeRef->getOpType();

         vector<string> keyValues;
         keyValues = get_service_values(sd, lkey);

         //We only test if there are attribute value(s)
         //unless we're testing for existance
         if ( (keyValues.size() > 0) ||
              (op_type == sdTokenTypes::K_NULL) )
         {
            switch (op_type)
            {
               //Processing for the 'IN' operator
               case sdTokenTypes::K_IN:
               {
                  result = process_data_in(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::K_LIKE:
               {
                  result = process_data_like(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::K_NULL:
               {
                  result = process_data_null(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::EQ:
               {
                  result = process_data_eq(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::NE:
               {
                  result = process_data_ne(keyValues, nodeRef);
                  break;
               }
            }
         }
         nodeRef->setEvalResult(result);
      }

      if (top->getFirstChild() != NULL) {
         //cout << "evaluating Lchild " << endl;
         eval_service_filter(top->getFirstChild(), sd);
      }

      if (top->getNextSibling()) {
         //cout << "evaluating Rchild " << endl;
         eval_service_filter(top->getNextSibling(), sd);
      }

      //Now that leaf nodes are visited - apply logical OR/AND on the results
      //INTERNAL NODE
      if (tok_type == sdTokenTypes::K_OR || tok_type == sdTokenTypes::K_AND)
      {
         RefAST kid = top->getFirstChild();
         ((Refsd_node)top)->setEvalResult(((Refsd_node)kid)->getEvalResult());
         RefAST sib = kid->getNextSibling();
         while(sib)
         {
            bool res = ((Refsd_node)sib)->getEvalResult();
            if (tok_type == sdTokenTypes::K_OR)
               ((Refsd_node)top)->setEvalResult(((Refsd_node)top)->getEvalResult() || res);
            else
               ((Refsd_node)top)->setEvalResult(((Refsd_node)top)->getEvalResult() && res);
            sib = sib->getNextSibling();
         }
      }

      //Deal with the NOTs
      if ( tok_type == sdTokenTypes::K_NOT )
      {
         RefAST kid = top->getFirstChild();

         if ( kid == NULL )
         {
            //Trouble in query land
            SAGA_THROW("Invalid query.  NOT clause had no children",
                       saga::BadParameter);
         }

         //Get the result and NOT it
         ((Refsd_node)top)->setEvalResult(!((Refsd_node)kid)->getEvalResult());

         //Check things are OK.  Not should only have 1 child
         RefAST sib = kid->getNextSibling();
         if ( sib != NULL )
         {
            //Trouble in query land
            SAGA_THROW("Invalid query.  NOT clause had more than one child",
                       saga::BadParameter);
         }
      }
   }
}

void sd_bdii_query::eval_authz_filter(RefAST top,
                                   saga::sd::service_description& sd,
                                   bool allFlag,
                                   const std::set<std::string>& voSet,
                                   const std::set<std::string>& vomsSet,
                                   const std::set<std::string>& fqanSet,
                                   const std::set<std::string>& dnSet)
{
   std::set<std::string> emptySet;

   int tok_type;

   if (top != NULL)
   {
      tok_type = top->getType();
      //cout << "Token type:" << tok_type << endl;

      // LEAF NODE
      if (tok_type == sdTokenTypes::SVC_CMP)
      {
         Refsd_leafnode nodeRef;
         nodeRef = (Refsd_leafnode)top;
         bool result = false;

         int lkey = nodeRef->getLValue();
         int op_type = nodeRef->getOpType();

         vector<string> keyValues;
         std::set<std::string>::const_iterator beginIter;
         std::set<std::string>::const_iterator endIter;

         switch ( lkey )
         {
            case sdTokenTypes::K_VO:
               beginIter = voSet.begin();
               endIter = voSet.end();
            break;

            case sdTokenTypes::K_VOMS:
               beginIter = vomsSet.begin();
               endIter = vomsSet.end();
            break;

            case sdTokenTypes::K_FQAN:
               beginIter = fqanSet.begin();
               endIter = fqanSet.end();
            break;

            case sdTokenTypes::K_DN:
               beginIter = dnSet.begin();
               endIter = dnSet.end();
            break;

            default:
               //Leave the vector empty
               beginIter = emptySet.begin();
               endIter = emptySet.end();
            break;
         }

         //Copy our values in to our vector
         while ( beginIter != endIter )
         {
            keyValues.push_back(*beginIter);
            ++beginIter;
         }

         //We only test if there are attribute value(s)
         //unless we're testing for existance
         if ( (keyValues.size() > 0) ||
              (op_type == sdTokenTypes::K_NULL) ||
              (allFlag == true) )
         {
            switch (op_type)
            {
               //Processing for the 'IN' operator
               case sdTokenTypes::K_IN:
               {
                  result = process_data_in(keyValues, nodeRef, allFlag);
                  break;
               }

               case sdTokenTypes::K_LIKE:
               {
                  result = process_data_like(keyValues, nodeRef, allFlag);
                  break;
               }

               case sdTokenTypes::K_NULL:
               {
                  result = process_data_null(keyValues, nodeRef, allFlag);
                  break;
               }

               case sdTokenTypes::EQ:
               {
                  result = process_data_eq(keyValues, nodeRef, allFlag);
                  break;
               }

               case sdTokenTypes::NE:
               {
                  result = process_data_ne(keyValues, nodeRef, allFlag);
                  break;
               }
            }
         }
         nodeRef->setEvalResult(result);
      }

      if (top->getFirstChild() != NULL) {
         //cout << "evaluating Lchild " << endl;
         eval_authz_filter(top->getFirstChild(),
                           sd,
                           allFlag,
                           voSet,
                           vomsSet,
                           fqanSet,
                           dnSet);
      }

      if (top->getNextSibling()) {
         //cout << "evaluating Rchild " << endl;
         eval_authz_filter(top->getNextSibling(),
                           sd,
                           allFlag,
                           voSet,
                           vomsSet,
                           fqanSet,
                           dnSet);
      }

      //Now that leaf nodes are visited - apply logical OR/AND on the results
      //INTERNAL NODE
      if (tok_type == sdTokenTypes::K_OR || tok_type == sdTokenTypes::K_AND)
      {
         RefAST kid = top->getFirstChild();
         ((Refsd_node)top)->setEvalResult(((Refsd_node)kid)->getEvalResult());
         RefAST sib = kid->getNextSibling();
         while(sib)
         {
            bool res = ((Refsd_node)sib)->getEvalResult();
            if (tok_type == sdTokenTypes::K_OR)
               ((Refsd_node)top)->setEvalResult(((Refsd_node)top)->getEvalResult() || res);
            else
               ((Refsd_node)top)->setEvalResult(((Refsd_node)top)->getEvalResult() && res);
            sib = sib->getNextSibling();
         }
      }

      //Deal with the NOTs
      if ( tok_type == sdTokenTypes::K_NOT )
      {
         RefAST kid = top->getFirstChild();

         if ( kid == NULL )
         {
            //Trouble in query land
            SAGA_THROW("Invalid query.  NOT clause had no children",
                       saga::BadParameter);
         }

         //Get the result and NOT it
         ((Refsd_node)top)->setEvalResult(!((Refsd_node)kid)->getEvalResult());

         //Check things are OK.  Not should only have 1 child
         RefAST sib = kid->getNextSibling();
         if ( sib != NULL )
         {
            //Trouble in query land
            SAGA_THROW("Invalid query.  NOT clause had more than one child",
                       saga::BadParameter);
         }
      }
   }
}

void sd_bdii_query::eval_data_filter(RefAST top, saga::sd::service_data& sd)
{
   int tok_type;

   if (top != NULL)
   {
      tok_type = top->getType();
      //cout << "Token type:" << tok_type << endl;

      // LEAF NODE
      if (tok_type == sdTokenTypes::DATA_CMP)
      {
         Refsd_leafnode nodeRef;
         nodeRef = (Refsd_leafnode)top;
         bool result = false;

         string lkey = nodeRef->getLKey();
         int op_type = nodeRef->getOpType();

         vector<string> keyValues;
         if ( sd.attribute_exists(lkey) )
         {
            //Vector or scalar attribute?
            if ( sd.attribute_is_vector(lkey) )
            {
               keyValues = sd.get_vector_attribute(lkey);
            }

            else
            {
               keyValues.push_back(sd.get_attribute(lkey));
            }
         }

         //We only test if there are data attributes
         //unless we're testing for existance
         if ( (keyValues.size() > 0) ||
              (op_type == sdTokenTypes::K_NULL) )
         {
            switch (op_type)
            {
               //Processing for the 'IN' operator
               case sdTokenTypes::K_IN:
               {
                  result = process_data_in(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::K_LIKE:
               {
                  result = process_data_like(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::K_NULL:
               {
                  result = process_data_null(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::EQ:
               {
                  result = process_data_eq(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::NE:
               {
                  result = process_data_ne(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::LESS:
               {
                  result = process_data_lt(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::GT:
               {
                  result = process_data_gt(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::LE:
               {
                  result = process_data_le(keyValues, nodeRef);
                  break;
               }

               case sdTokenTypes::GE:
               {
                  result = process_data_ge(keyValues, nodeRef);
                  break;
               }
            }
         }
         nodeRef->setEvalResult(result);
      }

      if (top->getFirstChild() != NULL) {
         //cout << "evaluating Lchild " << endl;
         eval_data_filter(top->getFirstChild(), sd);
      }

      if (top->getNextSibling()) {
         //cout << "evaluating Rchild " << endl;
         eval_data_filter(top->getNextSibling(), sd);
      }

      //Now that leaf nodes are visited - apply logical OR/AND on the results
      //INTERNAL NODE
      if (tok_type == sdTokenTypes::K_OR || tok_type == sdTokenTypes::K_AND)
      {
         RefAST kid = top->getFirstChild();
         ((Refsd_node)top)->setEvalResult(((Refsd_node)kid)->getEvalResult());
         RefAST sib = kid->getNextSibling();
         while(sib)
         {
            bool res = ((Refsd_node)sib)->getEvalResult();
            if (tok_type == sdTokenTypes::K_OR)
               ((Refsd_node)top)->setEvalResult(((Refsd_node)top)->getEvalResult() || res);
            else
               ((Refsd_node)top)->setEvalResult(((Refsd_node)top)->getEvalResult() && res);
            sib = sib->getNextSibling();
         }
      }

      //Deal with the NOTs
      if ( tok_type == sdTokenTypes::K_NOT )
      {
         RefAST kid = top->getFirstChild();

         if ( kid == NULL )
         {
            //Trouble in query land
            SAGA_THROW("Invalid query.  NOT clause had no children",
                       saga::BadParameter);
         }

         //Get the result and NOT it
         ((Refsd_node)top)->setEvalResult(!((Refsd_node)kid)->getEvalResult());

         //Check things are OK.  Not should only have 1 child
         RefAST sib = kid->getNextSibling();
         if ( sib != NULL )
         {
            //Trouble in query land
            SAGA_THROW("Invalid query.  NOT clause had more than one child",
                       saga::BadParameter);
         }
      }
   }
}

//Test the LHS attribute against a list of values
bool sd_bdii_query::process_data_in(const std::vector<std::string>& lhs,
                                 Refsd_leafnode nodeRef,
                                 bool passFlag) const
{
   bool goodResult = false;
   bool notResult = false;

   sd_leafnode::ANYALLTYPE anyAll = nodeRef->getAnyValue();

   //Check if the 'IN' was NOTted
   //if so reverse the result
   if ( nodeRef->getNotPrefix() )
   {
      notResult = true;;
   }

   //Shortcut if passFlag is set
   //passFlag == true means everything passes (unless it's notted)
   if ( passFlag == true )
   {
      if ( notResult == true )
      {
         return false;
      }

      else
      {
         return true;
      }
   }

   //Create a new vector of everything in the 'IN' condition
   std::vector<std::string> inVec;

   //Loop through everything in the 'IN' list
   std::list<std::string>::const_iterator iter = nodeRef->getListBegin();
   std::list<std::string>::const_iterator endIter = nodeRef->getListEnd();

   while ( iter != endIter )
   {
      std::string strVal = *iter;

      //Get rid of any leading and trailing quotes
      RemoveOuterQuotes(strVal);

      //Add this to the vector
      inVec.push_back(strVal);

      ++iter;
   }

   if ( inVec.size() == 0 )
   {
      //We might have a single item rather than a list
      //so deal with that here
      if ( nodeRef->getRValType() == sdTokenTypes::S_NUMBER )
      {
         std::ostringstream oss;
         oss << nodeRef->getRValNum();
         inVec.push_back(oss.str());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         RemoveOuterQuotes(strVal);
         inVec.push_back(strVal);
      }

   }
   //Now loop through each value and search for it in the 'IN' vector
   std::vector<std::string>::const_iterator keyIter;
   std::vector<std::string>::const_iterator endKeyIter = lhs.end();

   //Iterators into our 'IN' vector
   std::vector<std::string>::const_iterator inIter;
   std::vector<std::string>::const_iterator beginInIter = inVec.begin();
   std::vector<std::string>::const_iterator endInIter = inVec.end();

   for ( keyIter = lhs.begin();
         keyIter != endKeyIter;
         ++keyIter )
   {
      std::string lhsValue = *keyIter;

      //Look for LHS value in 'IN' list
      inIter = find(beginInIter, endInIter, lhsValue);

      //Did we get a "good" result?
      //Found for an 'IN'
      //Not found for a 'NOT IN'
      if ( ((inIter == endInIter) && (notResult == true)) ||
           ((inIter != endInIter) && (notResult == false)) )
      {
         goodResult = true;
      }

      else
      {
         goodResult = false;
      }

      //For an ANY query we only need one value to pass
      if ( (anyAll == sd_leafnode::ANY_VALUE) &&
           (goodResult == true) )
      {
         return true;
      }

      //For an ALL query we need all values to pass
      else if ( (anyAll == sd_leafnode::ALL_VALUE) &&
                (goodResult == false) )
      {
         return false;
      }
   }

   //If we get here then everything has been checked
   //No ANY has passed and no ALL has failed
   if ( anyAll == sd_leafnode::ANY_VALUE )
   {
      return false;
   }

   else if ( anyAll == sd_leafnode::ALL_VALUE )
   {
      return true;
   }

   else
   {
      return false;
   }
}

//Test the LHS attribute against a regex
bool sd_bdii_query::process_data_like(const std::vector<std::string>& lhs,
                                   Refsd_leafnode nodeRef,
                                   bool passFlag) const
{
   bool notResult = false;

   //Check if the 'LIKE' was NOTted
   //if so reverse the result
   if ( nodeRef->getNotPrefix() )
   {
      notResult = true;
   }

   //Shortcut if passFlag is set
   //passFlag == true means everything passes (unless it's notted)
   if ( passFlag == true )
   {
      if ( notResult == true )
      {
         return false;
      }

      else
      {
         return true;
      }
   }

   bool tmpResult = false;

   std::string likeStr = nodeRef->getRValue();

   //Get rid of any leading and trailing quotes
   RemoveOuterQuotes(likeStr);

   //Get escape character for 'LIKE' query
   char escapeChar = nodeRef->getEscapeChar();

   //Need to convert likeStr to a regex
   std::string regexStr = CreateRegexString(likeStr, escapeChar);

   //Create our regex and do the comparison
   boost::regex regExpr(regexStr,
                        boost::regex::basic | boost::regex::icase);

   std::vector<std::string>::const_iterator keyIter;
   std::vector<std::string>::const_iterator endKeyIter = lhs.end();

   for ( keyIter = lhs.begin();
         keyIter != endKeyIter;
         ++keyIter )
   {
      std::string lhsValue = *keyIter;

      //tmpResult starts false, so any match here
      //will make it true
      tmpResult |= regex_match(lhsValue, regExpr);
   }

   //Check if the 'LIKE' was NOTted
   //if so reverse the result
   if ( notResult == true )
   {
      tmpResult = !tmpResult;
   }

   return tmpResult;
}

//Test the LHS attribute for non-existance
bool sd_bdii_query::process_data_null(const std::vector<std::string>& lhs,
                                   Refsd_leafnode nodeRef,
                                   bool passFlag) const
{
   bool notResult = false;

   //Check if the 'is null' was NOTted
   //if so reverse the result
   if ( nodeRef->getNotPrefix() )
   {
      notResult = true;
   }

   //Shortcut if passFlag is set
   //passFlag == true means everything exists and is not null
   //(unless it's notted)
   if ( passFlag == true )
   {
      if ( notResult == true )
      {
         return true;
      }

      else
      {
         return false;
      }
   }

   //If this data attribute exists then "IS NULL" is false
   bool tmpResult = (lhs.size() == 0) ? true : false;

   //Check if the 'IS NULL' was NOTted
   //if so reverse the result
   if ( notResult == true )
   {
      tmpResult = !tmpResult;
   }

   return tmpResult;
}

//Test the LHS attribute for equality with the RHS
//Intentional copy by value
bool sd_bdii_query::process_data_eq(std::vector<std::string> lhs,
                                 Refsd_leafnode nodeRef,
                                 bool passFlag) const
{
   //Shortcut if passFlag is set
   //passFlag == true means everything matches, unless it's notted)
   if ( passFlag == true )
   {
      return true;
   }

   //Create two vectors, one for the LHS and one for the RHS
   //Each vector is ordered and contains only unique entries

   //Create the LHS vector
   std::vector<std::string> lhsVec;
   sort(lhs.begin(), lhs.end());
   unique_copy(lhs.begin(), lhs.end(), back_inserter(lhsVec));

   //Create the RHS vector
   std::vector<std::string> tmpRhsVec;
   std::vector<std::string> rhsVec;

   //Loop through everything in the 'EQ' list
   std::list<std::string>::const_iterator iter = nodeRef->getListBegin();
   std::list<std::string>::const_iterator endIter = nodeRef->getListEnd();

   while ( iter != endIter )
   {
      std::string strVal = *iter;

      //Get rid of any leading and trailing quotes
      RemoveOuterQuotes(strVal);

      //Add this to our output list
      tmpRhsVec.push_back(strVal);

      ++iter;
   }

   if ( tmpRhsVec.size() == 0 )
   {
      //We might have a single item rather than a list
      //so deal with that here
      if ( nodeRef->getRValType() == sdTokenTypes::S_NUMBER )
      {
         std::ostringstream oss;
         oss << nodeRef->getRValNum();
         tmpRhsVec.push_back(oss.str());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         RemoveOuterQuotes(strVal);
         tmpRhsVec.push_back(strVal);
      }

   }
   sort(tmpRhsVec.begin(), tmpRhsVec.end());
   unique_copy(tmpRhsVec.begin(), tmpRhsVec.end(), back_inserter(rhsVec));

   //Now compare our two vectors
   return ( lhsVec == rhsVec );
}

//Test the LHS attribute for non-equality with the RHS
//Intentional copy by value
bool sd_bdii_query::process_data_ne(std::vector<std::string> lhs,
                                 Refsd_leafnode nodeRef,
                                 bool passFlag) const
{
   //Shortcut if passFlag is set
   //passFlag == true means nothing matches, unless it's notted)
   if ( passFlag == true )
   {
      return false;
   }

   //Create two vectors, one for the LHS and one for the RHS
   //Each vector is ordered and contains only unique entries

   //Create the LHS vector
   std::vector<std::string> lhsVec;
   sort(lhs.begin(), lhs.end());
   unique_copy(lhs.begin(), lhs.end(), back_inserter(lhsVec));

   //Create the RHS vector
   std::vector<std::string> tmpRhsVec;
   std::vector<std::string> rhsVec;

   //Loop through everything in the 'NE' list
   std::list<std::string>::const_iterator iter = nodeRef->getListBegin();
   std::list<std::string>::const_iterator endIter = nodeRef->getListEnd();

   while ( iter != endIter )
   {
      std::string strVal = *iter;

      //Get rid of any leading and trailing quotes
      RemoveOuterQuotes(strVal);

      //Add this to our output list
      tmpRhsVec.push_back(strVal);

      ++iter;
   }

   if ( tmpRhsVec.size() == 0 )
   {
      //We might have a single item rather than a list
      //so deal with that here
      if ( nodeRef->getRValType() == sdTokenTypes::S_NUMBER )
      {
         std::ostringstream oss;
         oss << nodeRef->getRValNum();
         tmpRhsVec.push_back(oss.str());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         RemoveOuterQuotes(strVal);
         tmpRhsVec.push_back(strVal);
      }

   }
   sort(tmpRhsVec.begin(), tmpRhsVec.end());
   unique_copy(tmpRhsVec.begin(), tmpRhsVec.end(), back_inserter(rhsVec));

   //Now compare our two vectors
   return ( lhsVec != rhsVec );
}

//Test the LHS attribute is LESS THAN the RHS
bool sd_bdii_query::process_data_lt(const std::vector<std::string>& lhs,
                                 Refsd_leafnode nodeRef) const
{
   bool tmpResult;

   sd_leafnode::ANYALLTYPE anyAll = nodeRef->getAnyValue();

   std::vector<std::string>::const_iterator keyIter;
   std::vector<std::string>::const_iterator endKeyIter = lhs.end();

   for ( keyIter = lhs.begin();
         keyIter != endKeyIter;
         ++keyIter )
   {
      std::string lhs_value = *keyIter;

      //Are we comparing a string or a number?
      if ( nodeRef->getRValType() == sdTokenTypes::S_NUMBER )
      {
         tmpResult = (atof(lhs_value.c_str()) < nodeRef->getRValNum());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         RemoveOuterQuotes(strVal);
         tmpResult = (lhs_value < strVal);
      }

      //For an ANY query we only need one value to pass
      if ( (anyAll == sd_leafnode::ANY_VALUE) &&
           (tmpResult == true) )
      {
         return true;
      }

      //For an ALL query we need all values to pass
      else if ( (anyAll == sd_leafnode::ALL_VALUE) &&
                (tmpResult == false) )
      {
         return false;
      }
   }

   //If we get here then everything has been checked
   //No ANY has passed and no ALL has failed
   if ( anyAll == sd_leafnode::ANY_VALUE )
   {
      return false;
   }

   else if ( anyAll == sd_leafnode::ALL_VALUE )
   {
      return true;
   }

   else
   {
      return false;
   }
}

//Test the LHS attribute is GREATER THAN the RHS
bool sd_bdii_query::process_data_gt(const std::vector<std::string>& lhs,
                                 Refsd_leafnode nodeRef) const
{
   bool tmpResult;

   sd_leafnode::ANYALLTYPE anyAll = nodeRef->getAnyValue();

   std::vector<std::string>::const_iterator keyIter;
   std::vector<std::string>::const_iterator endKeyIter = lhs.end();

   for ( keyIter = lhs.begin();
         keyIter != endKeyIter;
         ++keyIter )
   {
      std::string lhs_value = *keyIter;

      //Are we comparing a string or a number?
      if ( nodeRef->getRValType() == sdTokenTypes::S_NUMBER )
      {
         tmpResult = (atof(lhs_value.c_str()) > nodeRef->getRValNum());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         RemoveOuterQuotes(strVal);
         tmpResult = (lhs_value > strVal);
      }

      //For an ANY query we only need one value to pass
      if ( (anyAll == sd_leafnode::ANY_VALUE) &&
           (tmpResult == true) )
      {
         return true;
      }

      //For an ALL query we need all values to pass
      else if ( (anyAll == sd_leafnode::ALL_VALUE) &&
                (tmpResult == false) )
      {
         return false;
      }
   }

   //If we get here then everything has been checked
   //No ANY has passed and no ALL has failed
   if ( anyAll == sd_leafnode::ANY_VALUE )
   {
      return false;
   }

   else if ( anyAll == sd_leafnode::ALL_VALUE )
   {
      return true;
   }

   else
   {
      return false;
   }
}

//Test the LHS attribute is LESS THAN OR EQUAL TO the RHS
bool sd_bdii_query::process_data_le(const std::vector<std::string>& lhs,
                                 Refsd_leafnode nodeRef) const
{
   bool tmpResult;

   sd_leafnode::ANYALLTYPE anyAll = nodeRef->getAnyValue();

   std::vector<std::string>::const_iterator keyIter;
   std::vector<std::string>::const_iterator endKeyIter = lhs.end();

   for ( keyIter = lhs.begin();
         keyIter != endKeyIter;
         ++keyIter )
   {
      std::string lhs_value = *keyIter;

      //Are we comparing a string or a number?
      if ( nodeRef->getRValType() == sdTokenTypes::S_NUMBER )
      {
         tmpResult = (atof(lhs_value.c_str()) <= nodeRef->getRValNum());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         RemoveOuterQuotes(strVal);
         tmpResult = (lhs_value <= strVal);
      }

      //For an ANY query we only need one value to pass
      if ( (anyAll == sd_leafnode::ANY_VALUE) &&
           (tmpResult == true) )
      {
         return true;
      }

      //For an ALL query we need all values to pass
      else if ( (anyAll == sd_leafnode::ALL_VALUE) &&
                (tmpResult == false) )
      {
         return false;
      }
   }

   //If we get here then everything has been checked
   //No ANY has passed and no ALL has failed
   if ( anyAll == sd_leafnode::ANY_VALUE )
   {
      return false;
   }

   else if ( anyAll == sd_leafnode::ALL_VALUE )
   {
      return true;
   }

   else
   {
      return false;
   }
}

//Test the LHS attribute is MORE THAN OR EQUAL TO the RHS
bool sd_bdii_query::process_data_ge(const std::vector<std::string>& lhs,
                                 Refsd_leafnode nodeRef) const
{
   bool tmpResult;

   sd_leafnode::ANYALLTYPE anyAll = nodeRef->getAnyValue();

   std::vector<std::string>::const_iterator keyIter;
   std::vector<std::string>::const_iterator endKeyIter = lhs.end();

   for ( keyIter = lhs.begin();
         keyIter != endKeyIter;
         ++keyIter )
   {
      std::string lhs_value = *keyIter;

      //Are we comparing a string or a number?
      if ( nodeRef->getRValType() == sdTokenTypes::S_NUMBER )
      {
         tmpResult = (atof(lhs_value.c_str()) >= nodeRef->getRValNum());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         RemoveOuterQuotes(strVal);
         tmpResult = (lhs_value >= strVal);
      }

      //For an ANY query we only need one value to pass
      if ( (anyAll == sd_leafnode::ANY_VALUE) &&
           (tmpResult == true) )
      {
         return true;
      }

      //For an ALL query we need all values to pass
      else if ( (anyAll == sd_leafnode::ALL_VALUE) &&
                (tmpResult == false) )
      {
         return false;
      }
   }

   //If we get here then everything has been checked
   //No ANY has passed and no ALL has failed
   if ( anyAll == sd_leafnode::ANY_VALUE )
   {
      return false;
   }

   else if ( anyAll == sd_leafnode::ALL_VALUE )
   {
      return true;
   }

   else
   {
      return false;
   }
}

void sd_bdii_query::generate_svc_filter(RefAST top)
{
   int tok_type, ktoken, otoken;
   string kword, strVal;

   if ( top != NULL )
   {
      tok_type = top->getType();
      //cout << "Token type: " << tok_type << endl;

      // INTERNAL NODE OR
      if ( tok_type == sdTokenTypes::K_OR )
      {
         svc_filter.append("(|");
      }

      // INTERNAL NODE AND
      if ( tok_type == sdTokenTypes::K_AND )
      {
         svc_filter.append("(&");
      }

      // INTERNAL NODE NOT
      if ( tok_type == sdTokenTypes::K_NOT )
      {
         svc_filter.append("(!");
      }

      // LEAF NODE
      if ( tok_type == sdTokenTypes::SVC_CMP )
      {
         Refsd_leafnode nodeRef;

         nodeRef = (Refsd_leafnode)top;
         ktoken = nodeRef->getLValue();

         //GLUE1 does not have IMPLEMENTOR
         //so we need to work around this by ignoring it
         //We'll tidy up any invalid queries later, hopefully
         if ( ktoken != sdTokenTypes::K_IMPLEMENTOR )
         {
            kword = get_svc_keyword(ktoken);

            //Flag to show if we're dealing with Capability
            bool isCap = false;
            std::string capabilityStr;
            if ( ktoken == sdTokenTypes::K_CAPABILITY )
            {
               isCap = true;
            }

            //cout << kword << endl;
            otoken = nodeRef->getOpType();

            if ( nodeRef->getNotPrefix() )
            {
               svc_filter.append("(!");
            }

            if ( otoken == sdTokenTypes::K_IN )
            {
               svc_filter.append("(|");

               //Loop through everything in the 'IN' list
               std::list<std::string>::const_iterator iter =
                  nodeRef->getListBegin();
               std::list<std::string>::const_iterator endIter =
                  nodeRef->getListEnd();

               while ( iter != endIter )
               {
                  strVal = *iter;

                  //Get rid of any leading and trailing quotes
                  RemoveOuterQuotes(strVal);

                  if ( isCap == false )
                  {
                     svc_filter.append("("+ kword + "=" + strVal + ")");
                  }

                  else
                  {
                     svc_filter.append(get_capability_string(strVal));
                  }

                  ++iter;
               }
               svc_filter.append(")");
            }

            if ( otoken == sdTokenTypes::NE )
            {
               if ( nodeRef->getRValType() == sdTokenTypes::K_STRING_LIST )
               {
                  svc_filter.append("(!(&");
                  //Loop through everything in the 'EQ' list
                  std::list<std::string>::const_iterator iter =
                     nodeRef->getListBegin();
                  std::list<std::string>::const_iterator endIter =
                     nodeRef->getListEnd();

                  while ( iter != endIter )
                  {
                     strVal = *iter;

                     //Get rid of any leading and trailing quotes
                     RemoveOuterQuotes(strVal);

                     if ( isCap == false )
                     {
                        svc_filter.append("("+ kword + "=" + strVal + ")");
                     }

                     else
                     {
                        svc_filter.append(get_capability_string(strVal));
                     }

                     ++iter;
                  }
                  svc_filter.append("))");
               }

               else
               {
                  strVal = nodeRef->getRValue();
                  RemoveOuterQuotes(strVal);

                  if ( isCap == false )
                  {
                     svc_filter.append("(!(" + kword + "=" + strVal + "))");
                  }

                  else
                  {
                     svc_filter.append("(!(|" +
                                       get_capability_string(strVal) +
                                       "))");
                  }
               }
            }

            if ( otoken == sdTokenTypes::EQ )
            {
               if ( nodeRef->getRValType() == sdTokenTypes::K_STRING_LIST )
               {
                  svc_filter.append("(&");
                  //Loop through everything in the 'EQ' list
                  std::list<std::string>::const_iterator iter =
                     nodeRef->getListBegin();
                  std::list<std::string>::const_iterator endIter =
                     nodeRef->getListEnd();

                  while ( iter != endIter )
                  {
                     strVal = *iter;

                     //Get rid of any leading and trailing quotes
                     RemoveOuterQuotes(strVal);

                     if ( isCap == false )
                     {
                        svc_filter.append("("+ kword + "=" + strVal + ")");
                     }

                     else
                     {
                        svc_filter.append(get_capability_string(strVal));
                     }

                     ++iter;
                  }
                  svc_filter.append(")");
               }

               else
               {
                  strVal = nodeRef->getRValue();
                  RemoveOuterQuotes(strVal);

                  if ( isCap == false )
                  {
                     svc_filter.append("("+kword+"="+ strVal + ")");
                  }

                  else
                  {
                     svc_filter.append("(|" +
                                       get_capability_string(strVal) +
                                       ")");
                  }
               }
            }

            if ( otoken == sdTokenTypes::K_LIKE )
            {
               strVal = nodeRef->getRValue();
               RemoveOuterQuotes(strVal);
               char escapeChar = nodeRef->getEscapeChar();

               std::string searchStr = ConvertServiceString(strVal,
                     escapeChar);

               if ( isCap == false )
               {
                  svc_filter.append("("+kword+"="+searchStr+")");
               }

               else
               {
                  SAGA_THROW("Invalid query.  "
                             "LIKE clause not supported for Capability",
                             saga::BadParameter);
               }
            }

            if ( otoken == sdTokenTypes::K_NULL )
            {
               strVal = nodeRef->getRValue();

               svc_filter.append("(!("+kword+"=*))");
            }

            if ( nodeRef->getNotPrefix() )
            {
               svc_filter.append(")");
            }
         }
      }

      if ( top->getFirstChild() != NULL )
      {
         generate_svc_filter(top->getFirstChild());
         svc_filter.append(")");
      }

      if ( top->getNextSibling() )
      {
         generate_svc_filter(top->getNextSibling());
      }
   }
}

std::string sd_bdii_query::correct_svc_filter(const std::string& filter)
{
   std::string retVal = filter;

   size_t loc;

   //Remove any empty clause
   std::string empty("()");

   //Remove any empty NOT clause
   std::string emptyNot("(!)");

   //Remove any empty AND clause
   std::string emptyAnd("(&)");

   //Remove any empty OR clause
   std::string emptyOr("(|)");

   std::vector<string> removalVec;
   removalVec.push_back(empty);
   removalVec.push_back(emptyNot);
   removalVec.push_back(emptyAnd);
   removalVec.push_back(emptyOr);

   //Repeat until there are no more changes to make
   bool madeChange = true;

   while ( madeChange == true )
   {
      //Initially no changes have been made
      madeChange = false;

      std::vector<std::string>::const_iterator iter = removalVec.begin();

      while ( iter != removalVec.end() )
      {
         std::string searchStr = *iter;

         loc = retVal.find(searchStr);

         while ( loc != std::string::npos )
         {
            retVal.replace(loc, searchStr.length(), std::string());

            loc = retVal.find(searchStr);

            madeChange = true;
         }
         ++iter;
      }
   }
   return retVal;
}

void sd_bdii_query::generate_authz_filter(RefAST top)
{
   int tok_type, ktoken, otoken;
   string kword, kword2, kword3, strVal;

   kword2 = "GlueServiceAccessControlRule";
   kword3 = "GlueServiceAccessControlRuleBase";

   if ( top != NULL )
   {
      tok_type = top->getType();
      //cout << "Token type: " << tok_type << endl;

      // INTERNAL NODE OR
      if ( tok_type == sdTokenTypes::K_OR )
      {
         svc_filter.append("(|");
      }

      // INTERNAL NODE AND
      if ( tok_type == sdTokenTypes::K_AND )
      {
         svc_filter.append("(&");
      }

      // INTERNAL NODE NOT
      if ( tok_type == sdTokenTypes::K_NOT )
      {
         svc_filter.append("(!");
      }

      // LEAF NODE
      if ( tok_type == sdTokenTypes::SVC_CMP )
      {
         Refsd_leafnode nodeRef;

         nodeRef = (Refsd_leafnode)top;
         ktoken = nodeRef->getLValue();
         kword = get_svc_keyword(ktoken);
         //cout << kword << endl;
         otoken = nodeRef->getOpType();

         if ( nodeRef->getNotPrefix() )
         {
            svc_filter.append("(!");
         }

         if ( otoken == sdTokenTypes::K_IN )
         {
            svc_filter.append("(|");

            //Loop through everything in the 'IN' list
            std::list<std::string>::const_iterator iter =
               nodeRef->getListBegin();
            std::list<std::string>::const_iterator endIter =
               nodeRef->getListEnd();

            while ( iter != endIter )
            {
               strVal = *iter;

               //Get rid of any leading and trailing quotes
               RemoveOuterQuotes(strVal);
               svc_filter.append("(|");

               svc_filter.append("(&(" + kword + "=*)" +
                                  "("  + kword + "=" + strVal + "))");

               svc_filter.append("(&(" + kword2 + "=*)"
                                  "("  + kword2 + "=" + strVal + "))");

               svc_filter.append("(&(" + kword3 + "=*)" +
                                 "(|(" + kword3 + "=" + strVal + ")" +
                                   "(" + kword3 + "=VO:" + strVal + ")))");

               svc_filter.append(")");

               ++iter;
            }
            svc_filter.append(")");
         }

         /* In the following three cases the search filter
          * is a little longer than one might expect.
          * Due to the unique way that LDAP deals with non-existance
          * we have to check for existance and an explicit value
          * in each of the possible VO locations*/
         if ( otoken == sdTokenTypes::NE )
         {
            strVal = nodeRef->getRValue();
            RemoveOuterQuotes(strVal);
            svc_filter.append("(!");

            svc_filter.append("(|");

            svc_filter.append("(&(" + kword + "=*)" +
                               "("  + kword + "=" + strVal + "))");

            svc_filter.append("(&(" + kword2 + "=*)"
                               "("  + kword2 + "=" + strVal + "))");

            svc_filter.append("(&(" + kword3 + "=*)" +
                              "(|(" + kword3 + "=" + strVal + ")" +
                                "(" + kword3 + "=VO:" + strVal + ")))");

            svc_filter.append(")");
            svc_filter.append(")");
         }

         if ( otoken == sdTokenTypes::EQ )
         {
            if ( nodeRef->getRValType() == sdTokenTypes::K_STRING_LIST )
            {
               svc_filter.append("(&");
               //Loop through everything in the 'EQ' list
               std::list<std::string>::const_iterator iter =
                  nodeRef->getListBegin();
               std::list<std::string>::const_iterator endIter =
                  nodeRef->getListEnd();

               while ( iter != endIter )
               {
                  strVal = *iter;

                  //Get rid of any leading and trailing quotes
                  RemoveOuterQuotes(strVal);
                  svc_filter.append("(|");

                  svc_filter.append("(&(" + kword + "=*)" +
                                     "("  + kword + "=" + strVal + "))");

                  svc_filter.append("(&(" + kword2 + "=*)"
                                     "("  + kword2 + "=" + strVal + "))");

                  svc_filter.append("(&(" + kword3 + "=*)" +
                                    "(|(" + kword3 + "=" + strVal + ")" +
                                      "(" + kword3 + "=VO:" + strVal + ")))");

                  svc_filter.append(")");

                  ++iter;
               }
               svc_filter.append(")");
            }

            else
            {
                  strVal = nodeRef->getRValue();
                  RemoveOuterQuotes(strVal);
                  svc_filter.append("(|");

                  svc_filter.append("(&(" + kword + "=*)" +
                                     "("  + kword + "=" + strVal + "))");

                  svc_filter.append("(&(" + kword2 + "=*)"
                                     "("  + kword2 + "=" + strVal + "))");

                  svc_filter.append("(&(" + kword3 + "=*)" +
                                    "(|(" + kword3 + "=" + strVal + ")" +
                                      "(" + kword3 + "=VO:" + strVal + ")))");

                  svc_filter.append(")");
            }
         }

         if ( otoken == sdTokenTypes::K_LIKE )
         {
            strVal = nodeRef->getRValue();
            RemoveOuterQuotes(strVal);
            char escapeChar = nodeRef->getEscapeChar();

            std::string searchStr =
               ConvertServiceString(strVal, escapeChar);

            svc_filter.append("(|");

            svc_filter.append("(&(" + kword + "=*)" +
                               "("  + kword + "=" + searchStr + "))");

            svc_filter.append("(&(" + kword2 + "=*)"
                               "("  + kword2 + "=" + searchStr + "))");

            svc_filter.append("(&(" + kword3 + "=*)" +
                              "(|(" + kword3 + "=" + searchStr + ")" +
                                "(" + kword3 + "=VO:" + searchStr + ")))");

            svc_filter.append(")");
         }

         if ( otoken == sdTokenTypes::K_NULL )
         {
            strVal = nodeRef->getRValue();

            //Make the value lower case to aid our matching
            std::transform(strVal.begin(),
                           strVal.end(),
                           strVal.begin(),
                           static_cast<int(*)(int)>(std::tolower));

            //Nasty hack to deal with GLUE2 all keyword
            //Need also to reverse NOT flag
            if ( kword == "all" )
            {
               kword = "GlueServiceOwner";
               svc_filter.append("(|");
               svc_filter.append("(" + kword + "=*)");
               svc_filter.append("(" + kword2 + "=*)");
               svc_filter.append("(" + kword3 + "=*)");
               svc_filter.append(")");
            }

            else
            {
               svc_filter.append("(!(|");
               svc_filter.append("(" + kword + "=*)");
               svc_filter.append("(" + kword2 + "=*)");
               svc_filter.append("(" + kword3 + "=*)");
               svc_filter.append("))");
            }
         }

         if ( nodeRef->getNotPrefix() )
         {
            svc_filter.append(")");
         }
      }

      if ( top->getFirstChild() != NULL )
      {
         generate_authz_filter(top->getFirstChild());
         svc_filter.append(")");
      }

      if ( top->getNextSibling() )
      {
         generate_authz_filter(top->getNextSibling());
      }
   }
}

/* PUBLIC FUNCTIONS */

sd_bdii_query::sd_bdii_query(unsigned int glueVersion)
{
   svc_filter = "";
   vo_filter = "";
   data_filter = "";
   _glueVersion = glueVersion;

   initialize();
}

void sd_bdii_query::initialize()
{
   //Set up the capabilities multimap
   capability_map.insert(std::make_pair(
                            std::string("information.monitoring"),
                            std::string("org.glite.rgma.Browser")));
   capability_map.insert(std::make_pair(
                            std::string("information.monitoring"),
                            std::string("org.glite.rgma.Consumer")));
   capability_map.insert(std::make_pair(
                            std::string("information.monitoring"),
                            std::string("org.glite.rgma.OnDemandProducer")));
   capability_map.insert(std::make_pair(
                            std::string("information.monitoring"),
                            std::string("org.glite.rgma.PrimaryProducer")));
   capability_map.insert(std::make_pair(
                            std::string("information.monitoring"),
                            std::string("org.glite.rgma.SecondaryProducer")));
}

string sd_bdii_query::get_svc_filter(RefAST top)
{
   generate_svc_filter(top);

   //Due to there being no implementor in GLUE we may end up with
   //an invalid query so we'll try and fix it here
   svc_filter = correct_svc_filter(svc_filter);

   return svc_filter;
}

string sd_bdii_query::get_authz_filter(RefAST top)
{
   generate_authz_filter(top);
   return svc_filter;
}


bool sd_bdii_query::evaluate_service_filter(RefAST top,
                                         saga::sd::service_description& sd)
{
   eval_service_filter(top, sd);
   Refsd_node nodeRef = (Refsd_node)top;

   return nodeRef->getEvalResult();
}

bool sd_bdii_query::evaluate_authz_filter(RefAST top,
                                       saga::sd::service_description& sd,
                                       bool allFlag,
                                       const std::set<std::string>& voSet,
                                       const std::set<std::string>& vomsSet,
                                       const std::set<std::string>& fqanSet,
                                       const std::set<std::string>& dnSet)
{
   eval_authz_filter(top, sd, allFlag, voSet, vomsSet, fqanSet, dnSet);
   Refsd_node nodeRef = (Refsd_node)top;

   return nodeRef->getEvalResult();
}

bool sd_bdii_query::evaluate_data_filter (RefAST top, saga::sd::service_data& sd)
{
   eval_data_filter(top, sd);
   Refsd_node nodeRef = (Refsd_node)top;

   return nodeRef->getEvalResult();
}

