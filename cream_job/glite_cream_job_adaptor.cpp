//  Copyright (c) 2009-2010 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


// saga includes
#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

// saga adaptor includes
#include <saga/saga/adaptors/config.hpp>
#include <saga/saga/adaptors/adaptor.hpp>

// adaptor includes
#include "glite_cream_job_adaptor.hpp"
#include "glite_cream_job_service.hpp"
#include "glite_cream_job.hpp"

SAGA_ADAPTOR_REGISTER (glite_cream_job::adaptor);

namespace glite_cream_job
{
  //////////////////////////////////////////////////////////////////////////////
  // register function for the SAGA engine
  saga::impl::adaptor_selector::adaptor_info_list_type
    adaptor::adaptor_register (saga::impl::session * s)
  {
    // list of implemented cpi's
    saga::impl::adaptor_selector::adaptor_info_list_type list;

    // create empty preference list
    // these list should be filled with properties of the adaptor, 
    // which can be used to select adaptors with specific preferences.
    // Example:
    //   'security' -> 'gsi'
    //   'logging'  -> 'yes'
    //   'auditing' -> 'no'
    preference_type prefs; 

    // create file adaptor infos (each adaptor instance gets its own uuid)
    // and add cpi_infos to list
    job_service_cpi_impl::register_cpi (list, prefs, adaptor_uuid_);
    job_cpi_impl::register_cpi         (list, prefs, adaptor_uuid_);

    // and return list
    return (list);
  }
  
  //////////////////////////////////////////////////////////////////////////////
  //
  bool adaptor::register_job(std::string job_id, std::string x509_cert,
                             std::string delegation_name)
  {
    delegation_t del;
    
    del.first  = std::string(x509_cert);
    del.second = std::string(delegation_name);
    
    std::pair<delegation_map_t::iterator, bool> p =
        delegation_map_.insert(make_pair(job_id, del));
        
    return p.second;
  }
  
  //////////////////////////////////////////////////////////////////////////////
  //
  bool adaptor::unregister_job(std::string job_id)
  { 
    delegation_map_t::iterator it = delegation_map_.find(job_id);
    if (it == delegation_map_.end())
        return false;
    
    delegation_map_.erase(it);
    return true; 
  }

} // namespace glite_cream_job

