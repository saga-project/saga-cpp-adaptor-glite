//  Copyright (c) 2009 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// stl includes
#include <vector>

// saga includes
#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

// saga engine includes
#include <saga/impl/config.hpp>

// saga adaptor includes
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/attribute.hpp>

// saga package includes
#include <saga/saga/packages/job/adaptors/job.hpp>
#include <saga/saga/packages/job/adaptors/job_self.hpp>

#include <glite/ce/cream-client-api-c/VOMSWrapper.h>
using namespace glite::ce::cream_client_api::soap_proxy;
using namespace glite::ce::cream_client_api::util;

// adaptor includes
#include "glite_cream_job_service.hpp"
#include "glite_cream_job_utils.hpp"

////////////////////////////////////////////////////////////////////////
namespace glite_cream_job
{
  //////////////////////////////////////////////////////////////////////
  // constructor
  job_service_cpi_impl::job_service_cpi_impl (proxy                * p, 
                                              cpi_info const       & info,
                                              saga::ini::ini const & glob_ini, 
                                              saga::ini::ini const & adap_ini,
                                              TR1::shared_ptr <saga::adaptor> adaptor)
    : base_cpi (p, info, adaptor, cpi::Noflags)
  {
    instance_data data(this);
    
    // create a unique random delegation ID
    delegation_id = saga::uuid().string();
    
    // check if we can handle scheme
    if (!data->rm_.get_url().empty())
    {
        saga::url rm(data->rm_);
        std::string host(rm.get_host());

        std::string scheme(rm.get_scheme());

        if (scheme != "cream" && scheme !=  "any")
        {
            SAGA_OSSTREAM strm;
            strm << "Could not initialize job service for " << data->rm_ << ". "
                 << "Only cream:// and any:// schemes are supported by this adaptor.";
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }

        if (host.empty())
        {
            SAGA_OSSTREAM strm;
            strm << "Could not initialize job service for " << data->rm_ << ". "
                 << "URL doesn't define a hostname.";
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
    }
    else
    {
        SAGA_OSSTREAM strm;
        strm << "Could not initialize job service for " << data->rm_ << ". "
             << "No URL provided and resource discovery is not implemented yet.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm),
                           saga::adaptors::AdaptorDeclined);
    }

    // check if we have x.509 contexts available and if they are usable
    // with this adaptor. if no context is usable, the constructor fails with
    // an authorization failed exception.
    std::vector <saga::context> contexts = p->get_session ().list_contexts ();
    std::vector <saga::context> context_list;
    // holds a list of reasons why a context can't be used. if no context
    // can be used, the list will be appended to the exception message otherwise
    // it will be discarded. 
    std::vector <std::string> context_error_list;
    
    for (unsigned int i = 0; i < contexts.size (); i++)
    {
      // context_list contains a list of valid x509 VOMS contexts
      check_x509_voms_cert(contexts[i], context_list, context_error_list);
    } 
    
    if(context_list.size() <1) 
    {
        SAGA_OSSTREAM strm;
        strm << "Could not initialize job service for " << data->rm_ << ". "
             << "No valid and/or usable x.509 context could be found:\n";
        for(unsigned int i=0; i<context_error_list.size(); ++i) {
          strm << "    - " << context_error_list[i] << "\n";
        }
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm),
                           saga::AuthorizationFailed);
    }
    else
    {
      saga::url rm(data->rm_);
      
      // try to delegate all (?) valid proxies to the resource manager.
      for(unsigned int i = 0; i < context_list.size(); i++)
      {
        std::string errorMessage = "";
        std::string userproxy(context_list[i].get_attribute 
                             (saga::attributes::context_userproxy));
        
        saga::url serviceAddress;
        serviceAddress.set_host(rm.get_host());
        serviceAddress.set_scheme("https");
        serviceAddress.set_port(8443);
        serviceAddress.set_path("/ce-cream/services/gridsite-delegation");
          
        bool success = try_delegate_proxy(serviceAddress.get_url(), delegation_id, 
                                          userproxy, errorMessage);                                 
        if(!success)
        {
          SAGA_OSSTREAM strm;
          strm << "Could not delegate (id="<< delegation_id <<") userproxy " << userproxy << " to " << serviceAddress.get_url() << ". "
               << errorMessage;
          SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm),
                             saga::AuthorizationFailed);
        }
      } 
    }
    
  }

  ////////////////////////////////////////////////////////////////////////
  // destructor
  job_service_cpi_impl::~job_service_cpi_impl (void)
  {

  }

  //////////////////////////////////////////////////////////////////////
  // SAGA API functions
  void 
    job_service_cpi_impl::sync_create_job (saga::job::job         & ret, 
                                           saga::job::description   jd)
  {
    instance_data data(this);   

    saga::attribute attr (jd);
    // A job description needs at least an 'Executable' 
    // attribute. Doesn't make sense without one.
    if (!attr.attribute_exists(saga::job::attributes::description_executable) ||
        attr.get_attribute(saga::job::attributes::description_executable).empty())
    {
      SAGA_OSSTREAM strm;
		  strm << "Could not create a job object for " << data->rm_ << ". " 
           << "The job description is missing the mandatory 'executable' attribute.";
		  SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter); 
    }
    
    try {
      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_DEBUG) {
        std::cerr << DBG_PRFX << "JDL: " << glite_cream_job::create_jsl_from_sjd(jd) << std::endl;
      } 
    }
    catch(std::exception const & e)
    {
      SAGA_OSSTREAM strm;
		  strm << "Could not create a job object for " << data->rm_ << ". " 
           << e.what();
		  SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter); 
    }
    
    
    saga::job::job job = saga::adaptors::job(data->rm_, jd, 
                                             proxy_->get_session());
    ret = job;
  }

  void 
    job_service_cpi_impl::sync_run_job (saga::job::job     & ret, 
                                        std::string          cmd, 
                                        std::string          host, 
                                        saga::job::ostream & in, 
                                        saga::job::istream & out, 
                                        saga::job::istream & err)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void 
    job_service_cpi_impl::sync_list (std::vector <std::string> & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void
    job_service_cpi_impl::sync_get_job (saga::job::job & ret, 
                                        std::string      jobid)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void job_service_cpi_impl::sync_get_self (saga::job::self & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

} // namespace glite_cream_job
////////////////////////////////////////////////////////////////////////

