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
#include <map>
#include <vector>
#include <algorithm>

#include <boost/function_output_iterator.hpp>
#include <boost/assign/std/vector.hpp>

#include <saga/adaptors/config.hpp>
#include <saga/adaptors/adaptor.hpp>
#include <saga/adaptors/task.hpp>

#include "glite_isn_adaptor.hpp"
#include "glite_navigator.hpp"

using namespace glite_isn_adaptor;
using namespace saga;

SAGA_ADAPTOR_REGISTER (isn_adaptor);

/*
 * register function for the SAGA engine
 */
saga::impl::adaptor_selector::adaptor_info_list_type
   isn_adaptor::adaptor_register(saga::impl::session *s)
{
   // list of implemented cpi's
   saga::impl::adaptor_selector::adaptor_info_list_type list;

   // create preferences
   preference_type prefs; // (std::string ("security"), std::string ("none"));

   // create isn adaptor infos (each adaptor instance gets its own uuid)
   // and add cpi_infos to list
   navigator_cpi_impl::register_cpi(list, prefs, adaptor_uuid_);

   //Create a default security context if needed
   if ( s->is_default_session() )
   {
      using namespace boost::assign;
      typedef std::pair<std::string,
                        std::string> entry_type;
      std::vector<entry_type> entries;

      //Set up reasonable defaults for the context
      entries += entry_type(saga::attributes::context_type, "glite");

      s->add_proto_context(entries);
   }
   // and return list
   return (list);
}

