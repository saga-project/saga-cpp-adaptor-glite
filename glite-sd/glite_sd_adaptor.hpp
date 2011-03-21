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
#ifndef ADAPTORS_GLITE_SD_ADAPTOR_HPP

#define ADAPTORS_GLITE_SD_ADAPTOR_HPP

#include <map>
#include <saga/adaptors/adaptor.hpp>

//class discoverer_cpi_impl;

struct sd_adaptor : public saga::adaptor
{
   typedef saga::impl::v1_0::op_info         op_info;
   typedef saga::impl::v1_0::cpi_info        cpi_info;
   typedef saga::impl::v1_0::preference_type preference_type;

    std::string get_name (void) const
    {
        return BOOST_PP_STRINGIZE(SAGA_ADAPTOR_NAME);
    }

    /**
    * This functions registers the adaptor with the factory
    *
    * @param factory the factory where the adaptor registers
    *        its maker function and description table
    */
    saga::impl::adaptor_selector::adaptor_info_list_type
            adaptor_register (saga::impl::session *s);

};

#endif
