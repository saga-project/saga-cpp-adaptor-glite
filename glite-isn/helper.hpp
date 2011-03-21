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
#ifndef _HELPER_HPP_
#define _HELPER_HPP_

#include <xercesc/util/XMLString.hpp>

// ---------------------------------------------------------------------------
//  This is a simple class that lets us do easy (though not terribly efficient)
//  trancoding of XMLCh data to local code page for display.
// ---------------------------------------------------------------------------
class StrX
{
public :
   //Simple constructor that takes an XMLCh*
   StrX(const XMLCh* const toTranscode)
   {
      //Call the private transcoding method
      _localForm =
         XERCES_CPP_NAMESPACE_QUALIFIER XMLString::transcode(toTranscode);
   }

   //Copy constructor
   StrX(const StrX& other)
   {
      _localForm =
         XERCES_CPP_NAMESPACE_QUALIFIER XMLString::replicate(other._localForm);
   }

   //Exception safe assignment operator
   //implemented in terms of copy and swap
   StrX& operator=(const StrX& rhs)
   {
      //This is self-assignment safe
      //so we don't need to test for it
      StrX temp(rhs);
      Swap(temp);
      return *this;
   }

   ~StrX()
   {
      //Release the allocated string
      XERCES_CPP_NAMESPACE_QUALIFIER XMLString::release(&_localForm);
   }

   //Helper method to translate to a char*
   const char* localForm() const
   {
      return _localForm;
   }

private:
   //No default constructor or assignment operator
   StrX();

   //Non-throwing Swap function
   void Swap(StrX& other) throw()
   {
      std::swap(_localForm, other._localForm);
   }

   //This is the local code page form of the string
   char* _localForm;
};

inline std::ostream& operator<<(std::ostream& target, const StrX& toDump)
{
   target << toDump.localForm();
   return target;
}

#endif

