//  Copyright (c) 2009-2010 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_GLITE_CREAM_JOB_ADAPTOR_HPP
#define ADAPTORS_GLITE_CREAM_JOB_ADAPTOR_HPP

// saga adaptor includes
#include <saga/saga/adaptors/adaptor.hpp>

////////////////////////////////////////////////////////////////////////
namespace glite_cream_job
{
  struct adaptor : public saga::adaptor
  {
    typedef saga::impl::v1_0::op_info         op_info;  
    typedef saga::impl::v1_0::cpi_info        cpi_info;
    typedef saga::impl::v1_0::preference_type preference_type;
    
    ////////////////////////////////////////////////////////////////////
    // 
    typedef std::pair <std::string, std::string> delegation_t;
    typedef std::map <std::string, delegation_t> delegation_map_t;

    delegation_map_t delegation_map_;
    
    bool register_job(std::string job_id, std::string x509_cert,
                      std::string delegation_name);
                     
    bool unregister_job(std::string job_id);
    //
    ////////////////////////////////////////////////////////////////////
    
    
    // This function registers the adaptor with the factory
    // @param factory the factory where the adaptor registers
    //        its maker function and description table
    saga::impl::adaptor_selector::adaptor_info_list_type 
      adaptor_register (saga::impl::session * s);

    std::string get_name (void) const
    { 
      return BOOST_PP_STRINGIZE (SAGA_ADAPTOR_NAME);
    }
  };

} // namespace glite_cream_job
////////////////////////////////////////////////////////////////////////

#endif // ADAPTORS_GLITE_CREAM_JOB_ADAPTOR_HPP

