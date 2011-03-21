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
#ifndef GLITE_SD_PROXY_FUNCS_HPP
#define GLITE_SD_PROXY_FUNCS_HPP

#include <string>
#include <vector>
#include <ctime>
#include "boost/filesystem/path.hpp"

namespace glite_adaptor
{
   class proxy_funcs
   {
      static std::string GetProxyPath();

      ~proxy_funcs(void){}
      proxy_funcs(){_modified_time = 0;}
      proxy_funcs(const proxy_funcs& other);
      proxy_funcs& operator=(const proxy_funcs& rhs);

      boost::filesystem::path _path;
      std::string _identity;
      std::vector<std::string> _vo;
      std::vector<std::string> _fqan;
      std::time_t _modified_time;

      public:
         bool GetProxyAttributes(const std::string& proxy_cert_path,
                                 std::string& identity,
                                 std::vector<std::string>& vo,
                                 std::vector<std::string>& fqan,
                                 std::string& error_str);

         static proxy_funcs& Instance();

   };
};

#endif

