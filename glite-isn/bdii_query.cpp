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
#include "bdii_query.hpp"
#include <boost/regex.hpp>

#include <sstream>
#include <stdlib.h>

#include "isn_lexer.hpp"
#include "isn_parser_ext.hpp"
#include <antlr/AST.hpp>

#include "saga/saga/packages/isn/entity_data.hpp"
#include <saga/adaptors/adaptor.hpp>

#include "config_handler.hpp"

//Non-exported helper functions
//Don't need to be a part of the class but are useful here
namespace
{
   std::string create_regex_string(const std::string& filter,
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

   void remove_outer_quotes(std::string& inStr)
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
void bdii_query::eval_data_filter(RefAST top,
                                  saga::isn::entity_data& data,
                                  const std::map<std::string,
                                                 ENTITY_ATTR_MAP_TYPE>& attrs)
{
   int tok_type;

   if (top != NULL)
   {
      tok_type = top->getType();
      //cout << "Token type:" << tok_type << endl;

      // LEAF NODE
      if (tok_type == isnTokenTypes::DATA_CMP)
      {
         Refisn_leafnode nodeRef;
         nodeRef = (Refisn_leafnode)top;
         bool result = false;

         string lkey = nodeRef->getLKey();

         //Now we need to convert the LHS key to its LDAP equivalent
         std::map<std::string, ENTITY_ATTR_MAP_TYPE>::const_iterator iter =
            attrs.find(lkey);

         if ( iter == attrs.end() )
         {
            std::string err = "Invalid query.  Unknown attribute '" +
                              lkey +
                              "'";
            SAGA_THROW_NO_OBJECT(err, saga::BadParameter);
         }

         else
         {
            lkey = iter->first;
         }

         int op_type = nodeRef->getOpType();

         vector<string> keyValues;
         if ( data.attribute_exists(lkey) )
         {
            //Vector or scalar attribute?
            if ( data.attribute_is_vector(lkey) )
            {
               keyValues = data.get_vector_attribute(lkey);
            }

            else
            {
               keyValues.push_back(data.get_attribute(lkey));
            }
         }

         //We only test if there are data attributes
         //unless we're testing for existance
         if ( (keyValues.size() > 0) ||
              (op_type == isnTokenTypes::K_NULL) )
         {
            switch (op_type)
            {
               //Processing for the 'IN' operator
               case isnTokenTypes::K_IN:
               {
                  result = process_data_in(keyValues, nodeRef);
                  break;
               }

               case isnTokenTypes::K_LIKE:
               {
                  result = process_data_like(keyValues, nodeRef);
                  break;
               }

               case isnTokenTypes::K_NULL:
               {
                  result = process_data_null(keyValues, nodeRef);
                  break;
               }

               case isnTokenTypes::EQ:
               {
                  result = process_data_eq(keyValues, nodeRef);
                  break;
               }

               case isnTokenTypes::NE:
               {
                  result = process_data_ne(keyValues, nodeRef);
                  break;
               }

               case isnTokenTypes::LESS:
               {
                  result = process_data_lt(keyValues, nodeRef);
                  break;
               }

               case isnTokenTypes::GT:
               {
                  result = process_data_gt(keyValues, nodeRef);
                  break;
               }

               case isnTokenTypes::LE:
               {
                  result = process_data_le(keyValues, nodeRef);
                  break;
               }

               case isnTokenTypes::GE:
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
         eval_data_filter(top->getFirstChild(), data, attrs);
      }

      if (top->getNextSibling()) {
         //cout << "evaluating Rchild " << endl;
         eval_data_filter(top->getNextSibling(), data, attrs);
      }

      //Now that leaf nodes are visited - apply logical OR/AND on the results
      //INTERNAL NODE
      if (tok_type == isnTokenTypes::K_OR || tok_type == isnTokenTypes::K_AND)
      {
         RefAST kid = top->getFirstChild();
         ((Refisn_node)top)->setEvalResult(((Refisn_node)kid)->getEvalResult());
         RefAST sib = kid->getNextSibling();
         while(sib)
         {
            bool res = ((Refisn_node)sib)->getEvalResult();
            if (tok_type == isnTokenTypes::K_OR)
               ((Refisn_node)top)->setEvalResult(((Refisn_node)top)->getEvalResult() || res);
            else
               ((Refisn_node)top)->setEvalResult(((Refisn_node)top)->getEvalResult() && res);
            sib = sib->getNextSibling();
         }
      }

      //Deal with the NOTs
      if ( tok_type == isnTokenTypes::K_NOT )
      {
         RefAST kid = top->getFirstChild();

         if ( kid == NULL )
         {
            //Trouble in querey land
            SAGA_THROW_NO_OBJECT("Invalid querey.  NOT clause had no children",
                                 saga::NoSuccess);
         }

         //Get the result and NOT it
         ((Refisn_node)top)->setEvalResult(!((Refisn_node)kid)->getEvalResult());

         //Check things are OK.  Not should only have 1 child
         RefAST sib = kid->getNextSibling();
         if ( sib != NULL )
         {
            //Trouble in querey land
            SAGA_THROW_NO_OBJECT("Invalid querey.  "
                                 "NOT clause had more than one child",
                                 saga::NoSuccess);
         }
      }
   }
}

//Test the LHS attribute against a list of values
bool bdii_query::process_data_in(const std::vector<std::string>& lhs,
                                 Refisn_leafnode nodeRef)
{
   bool goodResult = false;
   bool notResult = false;

   isn_leafnode::ANYALLTYPE anyAll = nodeRef->getAnyValue();

   //Check if the 'IN' was NOTted
   //if so reverse the result
   if ( nodeRef->getNotPrefix() )
   {
      notResult = true;
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
      remove_outer_quotes(strVal);

      //Add this to the vector
      inVec.push_back(strVal);

      ++iter;
   }

   if ( inVec.size() == 0 )
   {
      //We might have a single item rather than a list
      //so deal with that here
      if ( nodeRef->getRValType() == isnTokenTypes::S_NUMBER )
      {
         std::ostringstream oss;
         oss << nodeRef->getRValNum();
         inVec.push_back(oss.str());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         remove_outer_quotes(strVal);
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
      if ( (anyAll == isn_leafnode::ANY_VALUE) &&
           (goodResult == true) )
      {
         return true;
      }

      //For an ALL query we need all values to pass
      else if ( (anyAll == isn_leafnode::ALL_VALUE) &&
                (goodResult == false) )
      {
         return false;
      }
   }

   //If we get here then everything has been checked
   //No ANY has passed and no ALL has failed
   if ( anyAll == isn_leafnode::ANY_VALUE )
   {
      return false;
   }

   else if ( anyAll == isn_leafnode::ALL_VALUE )
   {
      return true;
   }

   else
   {
      return false;
   }
}

//Test the LHS attribute against a regex
bool bdii_query::process_data_like(const std::vector<std::string>& lhs,
                                   Refisn_leafnode nodeRef)
{
   bool tmpResult = false;

   std::string likeStr = nodeRef->getRValue();

   //Get rid of any leading and trailing quotes
   remove_outer_quotes(likeStr);

   //Get escape character for 'LIKE' query
   char escapeChar = nodeRef->getEscapeChar();

   //Need to convert likeStr to a regex
   std::string regexStr = create_regex_string(likeStr, escapeChar);

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
   if ( nodeRef->getNotPrefix() )
   {
      tmpResult = !tmpResult;
   }

   return tmpResult;
}

//Test the LHS attribute for non-existance
bool bdii_query::process_data_null(const std::vector<std::string>& lhs,
                                   Refisn_leafnode nodeRef)
{
   //If this data attribute exists then "IS NULL" is false
   bool tmpResult = lhs.size() ? false : true;

   //Check if the 'IS NULL' was NOTted
   //if so reverse the result
   if ( nodeRef->getNotPrefix() )
   {
      tmpResult = !tmpResult;
   }

   return tmpResult;
}

//Test the LHS attribute for equality with the RHS
//Intentional copy by value
bool bdii_query::process_data_eq(std::vector<std::string> lhs,
                                 Refisn_leafnode nodeRef)
{
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
      remove_outer_quotes(strVal);

      //Add this to our output list
      tmpRhsVec.push_back(strVal);

      ++iter;
   }

   if ( tmpRhsVec.size() == 0 )
   {
      //We might have a single item rather than a list
      //so deal with that here
      if ( nodeRef->getRValType() == isnTokenTypes::S_NUMBER )
      {
         std::ostringstream oss;
         oss << nodeRef->getRValNum();
         tmpRhsVec.push_back(oss.str());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         remove_outer_quotes(strVal);
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
bool bdii_query::process_data_ne(std::vector<std::string> lhs,
                                 Refisn_leafnode nodeRef)
{
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
      remove_outer_quotes(strVal);

      //Add this to our output list
      tmpRhsVec.push_back(strVal);

      ++iter;
   }

   if ( tmpRhsVec.size() == 0 )
   {
      //We might have a single item rather than a list
      //so deal with that here
      if ( nodeRef->getRValType() == isnTokenTypes::S_NUMBER )
      {
         std::ostringstream oss;
         oss << nodeRef->getRValNum();
         tmpRhsVec.push_back(oss.str());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         remove_outer_quotes(strVal);
         tmpRhsVec.push_back(strVal);
      }

   }
   sort(tmpRhsVec.begin(), tmpRhsVec.end());
   unique_copy(tmpRhsVec.begin(), tmpRhsVec.end(), back_inserter(rhsVec));

   //Now compare our two vectors
   return ( lhsVec != rhsVec );
}

//Test the LHS attribute is LESS THAN the RHS
bool bdii_query::process_data_lt(const std::vector<std::string>& lhs,
                                 Refisn_leafnode nodeRef)
{
   bool tmpResult;

   isn_leafnode::ANYALLTYPE anyAll = nodeRef->getAnyValue();

   std::vector<std::string>::const_iterator keyIter;
   std::vector<std::string>::const_iterator endKeyIter = lhs.end();

   for ( keyIter = lhs.begin();
         keyIter != endKeyIter;
         ++keyIter )
   {
      std::string lhs_value = *keyIter;

      //Are we comparing a string or a number?
      if ( nodeRef->getRValType() == isnTokenTypes::S_NUMBER )
      {
         tmpResult = (atof(lhs_value.c_str()) < nodeRef->getRValNum());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         remove_outer_quotes(strVal);
         tmpResult = (lhs_value < strVal);
      }

      //For an ANY query we only need one value to pass
      if ( (anyAll == isn_leafnode::ANY_VALUE) &&
           (tmpResult == true) )
      {
         return true;
      }

      //For an ALL query we need all values to pass
      else if ( (anyAll == isn_leafnode::ALL_VALUE) &&
                (tmpResult == false) )
      {
         return false;
      }
   }

   //If we get here then everything has been checked
   //No ANY has passed and no ALL has failed
   if ( anyAll == isn_leafnode::ANY_VALUE )
   {
      return false;
   }

   else if ( anyAll == isn_leafnode::ALL_VALUE )
   {
      return true;
   }

   else
   {
      return false;
   }
}

//Test the LHS attribute is GREATER THAN the RHS
bool bdii_query::process_data_gt(const std::vector<std::string>& lhs,
                                 Refisn_leafnode nodeRef)
{
   bool tmpResult;

   isn_leafnode::ANYALLTYPE anyAll = nodeRef->getAnyValue();

   std::vector<std::string>::const_iterator keyIter;
   std::vector<std::string>::const_iterator endKeyIter = lhs.end();

   for ( keyIter = lhs.begin();
         keyIter != endKeyIter;
         ++keyIter )
   {
      std::string lhs_value = *keyIter;

      //Are we comparing a string or a number?
      if ( nodeRef->getRValType() == isnTokenTypes::S_NUMBER )
      {
         tmpResult = (atof(lhs_value.c_str()) > nodeRef->getRValNum());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         remove_outer_quotes(strVal);
         tmpResult = (lhs_value > strVal);
      }

      //For an ANY query we only need one value to pass
      if ( (anyAll == isn_leafnode::ANY_VALUE) &&
           (tmpResult == true) )
      {
         return true;
      }

      //For an ALL query we need all values to pass
      else if ( (anyAll == isn_leafnode::ALL_VALUE) &&
                (tmpResult == false) )
      {
         return false;
      }
   }

   //If we get here then everything has been checked
   //No ANY has passed and no ALL has failed
   if ( anyAll == isn_leafnode::ANY_VALUE )
   {
      return false;
   }

   else if ( anyAll == isn_leafnode::ALL_VALUE )
   {
      return true;
   }

   else
   {
      return false;
   }
}

//Test the LHS attribute is LESS THAN OR EQUAL TO the RHS
bool bdii_query::process_data_le(const std::vector<std::string>& lhs,
                                 Refisn_leafnode nodeRef)
{
   bool tmpResult;

   isn_leafnode::ANYALLTYPE anyAll = nodeRef->getAnyValue();

   std::vector<std::string>::const_iterator keyIter;
   std::vector<std::string>::const_iterator endKeyIter = lhs.end();

   for ( keyIter = lhs.begin();
         keyIter != endKeyIter;
         ++keyIter )
   {
      std::string lhs_value = *keyIter;

      //Are we comparing a string or a number?
      if ( nodeRef->getRValType() == isnTokenTypes::S_NUMBER )
      {
         tmpResult = (atof(lhs_value.c_str()) <= nodeRef->getRValNum());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         remove_outer_quotes(strVal);
         tmpResult = (lhs_value <= strVal);
      }

      //For an ANY query we only need one value to pass
      if ( (anyAll == isn_leafnode::ANY_VALUE) &&
           (tmpResult == true) )
      {
         return true;
      }

      //For an ALL query we need all values to pass
      else if ( (anyAll == isn_leafnode::ALL_VALUE) &&
                (tmpResult == false) )
      {
         return false;
      }
   }

   //If we get here then everything has been checked
   //No ANY has passed and no ALL has failed
   if ( anyAll == isn_leafnode::ANY_VALUE )
   {
      return false;
   }

   else if ( anyAll == isn_leafnode::ALL_VALUE )
   {
      return true;
   }

   else
   {
      return false;
   }
}

//Test the LHS attribute is MORE THAN OR EQUAL TO the RHS
bool bdii_query::process_data_ge(const std::vector<std::string>& lhs,
                                 Refisn_leafnode nodeRef)
{
   bool tmpResult;

   isn_leafnode::ANYALLTYPE anyAll = nodeRef->getAnyValue();

   std::vector<std::string>::const_iterator keyIter;
   std::vector<std::string>::const_iterator endKeyIter = lhs.end();

   for ( keyIter = lhs.begin();
         keyIter != endKeyIter;
         ++keyIter )
   {
      std::string lhs_value = *keyIter;

      //Are we comparing a string or a number?
      if ( nodeRef->getRValType() == isnTokenTypes::S_NUMBER )
      {
         tmpResult = (atof(lhs_value.c_str()) >= nodeRef->getRValNum());
      }
      else
      {
         std::string strVal = nodeRef->getRValue();
         remove_outer_quotes(strVal);
         tmpResult = (lhs_value >= strVal);
      }

      //For an ANY query we only need one value to pass
      if ( (anyAll == isn_leafnode::ANY_VALUE) &&
           (tmpResult == true) )
      {
         return true;
      }

      //For an ALL query we need all values to pass
      else if ( (anyAll == isn_leafnode::ALL_VALUE) &&
                (tmpResult == false) )
      {
         return false;
      }
   }

   //If we get here then everything has been checked
   //No ANY has passed and no ALL has failed
   if ( anyAll == isn_leafnode::ANY_VALUE )
   {
      return false;
   }

   else if ( anyAll == isn_leafnode::ALL_VALUE )
   {
      return true;
   }

   else
   {
      return false;
   }
}

/* PUBLIC FUNCTIONS */

bdii_query::bdii_query(const std::string& model,
                       const std::string& filter,
                       const std::string& type)
{
   _model = model;
   _filter = filter;
   _type = type;

   std::istringstream data_stream(filter, std::istringstream::in);
   try {
     isn_lexer data_lexer(data_stream);
     isn_parser_ext data_parser(data_lexer);
     ASTFactory ast_factory;
     data_parser.initializeASTFactory(ast_factory);
     data_parser.setASTFactory(&ast_factory);
     data_parser.entity_filter();

     if ( data_parser.getErrorFlag() == true )
     {
        SAGA_THROW_NO_OBJECT(std::string(data_parser.getErrorString()),
                             saga::BadParameter);
     }

     _top = data_parser.getAST();

   } catch (antlr::ANTLRException& e) {
     SAGA_THROW_NO_OBJECT("Unable to parse " + filter + " " + std::string(e.getMessage()),
                                saga::BadParameter);
   }
}

bool bdii_query::evaluate_data_filter(saga::isn::entity_data& data)
{
   if ( _filter.empty() )
   {
      return true;
   }

   std::string configDirectory = "/usr/etc/saga/isn/";
   if ( _model == "glue1" )
   {
      configDirectory += "glue1/";
   }

   else if ( _model == "glue2" )
   {
      configDirectory += "glue2/";
   }

   else
   {
      SAGA_THROW_NO_OBJECT("Information System model '" + _model + "' unknown",
                           saga::BadParameter);
   }

   ENTITY_ATTR_TYPE attr = config_handler::get_entity(_type, configDirectory);
   eval_data_filter(_top, data, attr.attrs);
   Refisn_node nodeRef = (Refisn_node)_top;

   return nodeRef->getEvalResult();
}
