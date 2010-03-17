//  Copyright (c) 2009 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// saga includes
#include <saga/saga.hpp>
#include <saga/saga/adaptors/task.hpp>

// saga adaptor icnludes
#include <saga/saga/adaptors/task.hpp>
#include <saga/saga/adaptors/attribute.hpp>
#include <saga/saga/adaptors/file_transfer_spec.hpp>

// saga engine includes
#include <saga/impl/config.hpp>
#include <saga/impl/exception_list.hpp>

// saga package includes
#include <saga/saga/packages/job/adaptors/job_self.hpp>
#include <saga/saga/packages/job/job_description.hpp>

// adaptor includes
#include "glite_cream_job.hpp"
#include "glite_cream_job_utils.hpp"
#include "glite_cream_job_istream.hpp"
#include "glite_cream_job_ostream.hpp"

// glite cream api includes
#include <glite/ce/cream-client-api-c/VOMSWrapper.h>
#include <glite/ce/cream-client-api-c/CreamProxyFactory.h>
#include <glite/ce/cream-client-api-c/JobDescriptionWrapper.h>
using namespace glite::ce::cream_client_api::soap_proxy;
using namespace glite::ce::cream_client_api::util;
namespace CreamAPI = glite::ce::cream_client_api::soap_proxy;


////////////////////////////////////////////////////////////////////////
namespace glite_cream_job
{

  // constructor
  job_cpi_impl::job_cpi_impl (proxy                           * p, 
                              cpi_info const                  & info,
                              saga::ini::ini const            & glob_ini, 
                              saga::ini::ini const            & adap_ini,
                              TR1::shared_ptr <saga::adaptor>   adaptor)
    : base_cpi  (p, info, adaptor, cpi::Noflags)
  {
    instance_data data(this);
    
    // check if we can handle scheme
    if (!data->rm_.get_url().empty())
    {
        saga::url rm(data->rm_);
        std::string host(rm.get_host());
        std::string scheme(rm.get_scheme());

        if (scheme != "cream" && scheme !=  "any")
        {
            SAGA_OSSTREAM strm;
            strm << "Could not initialize job object for " << data->rm_ << ". "
                 << "Only cream:// and any:// schemes are supported by this adaptor.";
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }

        if (host.empty())
        {
            SAGA_OSSTREAM strm;
            strm << "Could not initialize job object for " << data->rm_ << ". "
                 << "URL doesn't define a hostname.";
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
    }
    else
    {
        SAGA_OSSTREAM strm;
        strm << "Could not initialize job object for " << data->rm_ << ". "
             << "No URL provided and resource discovery is not implemented yet.";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm),
                           saga::adaptors::AdaptorDeclined);
    }
    
    // Let's extract the hidden delegation ID
    if (data->jd_.attribute_exists(saga::job::attributes::description_job_contact)) {
      this->delegate_id = data->jd_.get_attribute(saga::job::attributes::description_job_contact);
               
      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
        std::cerr << DBG_PRFX << "Extracted delegate ID " << this->delegate_id 
                  << " for this job. " << std::endl;
      }
      
    }
    else {
      SAGA_ADAPTOR_THROW("Unexpected error: Delegation ID is missing!", saga::NoSuccess);
    }
    
    // Inital job state is 'Unknown' since the job is not started yet.
    update_state(saga::job::Unknown);
    
    if (data->init_from_jobid_) 
    {
      // Job was constructed by the get_job factory method with a JobID. 
      // This means that we have to connect to an existing job. If we can 
      // connect to the job, we have to:
      //   - set the current state
      //   - try to reconstruct the job description
      SAGA_ADAPTOR_THROW ("Job Re-connection Not Implemented yet", saga::NotImplemented);
    } // init from job id
    else
    {
      // From now on the job is in 'New' state - ready to run!
      //update_state(saga::job::New);
      std::string jdl;
      
      try {
        jdl = glite_cream_job::create_jsl_from_sjd(data->jd_);
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_DEBUG) {
          std::cerr << DBG_PRFX << "Created JDL: " << jdl << std::endl;
        } 
      }
      catch(std::exception const & e)
      {
        SAGA_OSSTREAM strm;
		    strm << "Could not create a job object for " << data->rm_ << ". " 
             << e.what();
		    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter); 
      }
      
      // Let's try to register the job with the CREAM CE.
      bool autostart = false;
      std::map<std::string, std::string> properties;
      std::string localCreamJID;
      
      std::string leaseID = "";
      std::string delegationProxy = "";
      
      CreamAPI::JobDescriptionWrapper jd(jdl, this->delegate_id, leaseID, delegationProxy, autostart, "foo");
      
      CreamAPI::AbsCreamProxy::RegisterArrayRequest reqs;
      reqs.push_back( &jd );
      CreamAPI::AbsCreamProxy::RegisterArrayResult resp;
      
      int connection_timeout = 30;
      
      CreamAPI::AbsCreamProxy* creamClient = 
        CreamAPI::CreamProxyFactory::make_CreamProxyRegister(&reqs, &resp, connection_timeout);
      
      if(NULL == creamClient)
      {
        SAGA_ADAPTOR_THROW("Unexpected: creamClient pointer is NULL.", saga::NoSuccess);
      }
      
      
    } // init from jd
  
  }


  // destructor
  job_cpi_impl::~job_cpi_impl (void)
  {
  }


  //  SAGA API functions
  void job_cpi_impl::sync_get_state (saga::job::state & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void job_cpi_impl::sync_get_description (saga::job::description & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void job_cpi_impl::sync_get_job_id (std::string & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  // access streams for communication with the child
  void job_cpi_impl::sync_get_stdin (saga::job::ostream & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void job_cpi_impl::sync_get_stdout (saga::job::istream & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void job_cpi_impl::sync_get_stderr (saga::job::istream & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void job_cpi_impl::sync_checkpoint (saga::impl::void_t & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void job_cpi_impl::sync_migrate (saga::impl::void_t           & ret, 
                                   saga::job::description   jd)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void job_cpi_impl::sync_signal (saga::impl::void_t & ret, 
                                  int            signal)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }


  //  suspend the child process 
  void job_cpi_impl::sync_suspend (saga::impl::void_t & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  //  suspend the child process 
  void job_cpi_impl::sync_resume (saga::impl::void_t & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }


  //////////////////////////////////////////////////////////////////////
  // inherited from the task interface
  void job_cpi_impl::sync_run (saga::impl::void_t & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void job_cpi_impl::sync_cancel (saga::impl::void_t & ret, 
                                  double timeout)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  //  wait for the child process to terminate
  void job_cpi_impl::sync_wait (bool   & ret, 
                                double   timeout)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }
  
  
  /////////////////////////////////////////////////////////////////////
  // utility functions, etc... 
  void job_cpi_impl::update_state(saga::job::state newstate)
  {
    saga::monitorable monitor (this->proxy_);
    saga::adaptors::metric m (monitor.get_metric(saga::metrics::task_state));
    m.set_attribute(saga::attributes::metric_value, 
                    saga::adaptors::job_state_enum_to_value(newstate));
  }


} // namespace glite_cream_job
////////////////////////////////////////////////////////////////////////

