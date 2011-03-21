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
#ifndef ISN_PARSER_EXT_HPP
#define ISN_PARSER_EXT_HPP

#include "isn_parser.hpp"
#include <string>
#include <sstream>
class isn_parser_ext : public isn_parser
{
   public:

   isn_parser_ext(antlr::TokenStream& lexer) : isn_parser(lexer)
   {
      setErrorFlag(false);
      setErrorString("");
   }

   //Get the error flag
   bool getErrorFlag() const
   {
      return _errorFlag;
   }

   //Set the error flag
   void setErrorFlag(bool errorFlag)
   {
      _errorFlag = errorFlag;
   }

   //Get the error string
   std::string getErrorString() const
   {
      return _errorString;
   }

   //Set the error string
   void setErrorString(const std::string& errorString)
   {
      _errorString = errorString;
   }

   // Override the reportError function defined in the Parser base
   // class.
   void reportError(const std::string& s)
   {
      if ( getErrorFlag() == false )
      {
         setErrorFlag(true);

         std::ostringstream os;
 
         if ( getFilename()=="" )
         {
            os << "Error: " << s;
         }

         else
         {
            os << "Error in " << getFilename()
               << ": " << s;
         }
         setErrorString(os.str());
      }
   }

   // Override the reportError function defined in the Parser base
   // class.
   void reportError(const antlr::RecognitionException& ex)
   {
      if ( getErrorFlag() == false )
      {
         setErrorFlag(true);

         std::ostringstream os;
 
         if ( getFilename()=="" )
         {
            os << "Error: " << ex.toString();
         }

         else
         {
            os << "Error in " << getFilename()
               << ": " << ex.toString();
         }
         setErrorString(os.str());
      }
   }

   private:

   bool _errorFlag;
   std::string _errorString;
};

#endif

