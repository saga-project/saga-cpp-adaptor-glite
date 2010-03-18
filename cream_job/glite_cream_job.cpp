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

// boost includes
#include <boost/tokenizer.hpp>

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
    
    // Inital job state is 'Unknown' since the job is not started yet.
    update_state(saga::job::Unknown);
    
    if (!data->rm_.get_url().empty())
    {
        if (!can_handle_scheme(data->rm_))
        {
            SAGA_OSSTREAM strm;
            strm << "Could not initialize job service for " << data->rm_ << ". "
                 << "Only cream:// and any:// schemes are supported by this adaptor.";
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }

        if (!can_handle_hostname(data->rm_))
        {
            SAGA_OSSTREAM strm;
            strm << "Could not initialize job service for hostname: " << data->rm_ << ". ";
            SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::adaptors::AdaptorDeclined);
        }
        
        std::string batchsystem, queue;
        if(!get_batchsystem_and_queue_from_url(batchsystem, queue, data->rm_))
        {
            SAGA_OSSTREAM strm;
            strm << "Batchsystem and queue name need to be encoded in the url path: " 
                 << "cream://<host>[:<port>]/cream-<batchsystem>-<queue-name>.";
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
    
    // Let's extract the hidden delegation ID and x509 path
    if (data->jd_.attribute_exists(saga::job::attributes::description_job_contact)) 
    {
      std::string packed_str = data->jd_.get_attribute(saga::job::attributes::description_job_contact);
      
      bool success = unpack_delegate_and_userproxy(packed_str, this->delegate_, this->userproxy_);
      if(!success) {
        SAGA_OSSTREAM strm;
        strm << "Unexpected error: Could not unpack delegate id and userproxy from " << packed_str << ". ";
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::NoSuccess);
      }
      else {         
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
          std::cerr << DBG_PRFX << "Extracted delegate id " << this->delegate_
                    << " and userproxy: " << this->userproxy_ << "." << std::endl; }
      }
    }
    else 
    {
      SAGA_ADAPTOR_THROW("Unexpected error: Delegation id and userproxy are missing!", 
                         saga::NoSuccess);
    }
    
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
        jdl = glite_cream_job::create_jsl_from_sjd(data->jd_, data->rm_);
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_DEBUG) {
          std::cerr << DBG_PRFX << "Created JDL: " << jdl << std::endl; } 
      }
      catch(std::exception const & e)
      {
        SAGA_OSSTREAM strm;
		    strm << "Could not create a job object for " << data->rm_ << ": " 
             << e.what();
		    SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter); 
      }
      
      // Let's try to register the job with the CREAM CE.
      bool autostart = false;
      std::map<std::string, std::string> properties;
      std::string localCreamJID;
      
      std::string leaseID = "";
      std::string delegationProxy = "";
      
      // create a unique random internal job id
      this->internal_jobid_ = saga::uuid().string();
      
      CreamAPI::JobDescriptionWrapper jd(jdl, this->delegate_, leaseID, delegationProxy, autostart, internal_jobid_);
      
      CreamAPI::AbsCreamProxy::RegisterArrayRequest reqs;
      reqs.push_back( &jd );
      CreamAPI::AbsCreamProxy::RegisterArrayResult resp;
      
      int connection_timeout = 30;
      
      CreamAPI::AbsCreamProxy* creamClient = 
        CreamAPI::CreamProxyFactory::make_CreamProxyRegister(&reqs, &resp, connection_timeout);
      
      if(NULL == creamClient)
      {
        SAGA_ADAPTOR_THROW("Unexpected: creamClient pointer is NULL in job c'tor.", saga::NoSuccess);
      }

      try {
        creamClient->setCredential(this->userproxy_);
        creamClient->execute(saga_to_cream2_service_url(data->rm_.get_url()));
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_DEBUG) {
          std::cerr << DBG_PRFX << "Successfully registerd job with: " 
                    << saga_to_cream2_service_url(data->rm_.get_url()) << std::endl; } 
      }
      catch(std::exception const & e)
      {
        SAGA_ADAPTOR_THROW("Could not register job: "+e.what(), saga::NoSuccess);
        delete creamClient;
      }
      
      boost::tuple<bool, CreamAPI::JobIdWrapper, std::string> 
        registrationResponse = resp[this->internal_jobid_];
        
      if(CreamAPI::JobIdWrapper::OK != registrationResponse.get<0>())
      {
        SAGA_ADAPTOR_THROW("Could not register job: "+registrationResponse.get<2>(), saga::NoSuccess);
        delete creamClient;
      }
      else
      {
        std::string creamURL = registrationResponse.get<1>().getCreamURL();
        std::string creamJID = registrationResponse.get<1>().getCreamJobID();
        creamJID = creamURL + "/" + creamJID;
        
        update_state(saga::job::New);
        saga::adaptors::attribute attr (this);
        attr.set_attribute (saga::job::attributes::jobid, creamJID);

        delete creamClient;
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
    // todo: implement active query!  
    saga::monitorable monitor (this->proxy_);
    saga::metric m (monitor.get_metric(saga::metrics::task_state));
    ret = saga::adaptors::job_state_value_to_enum(m.get_attribute(saga::attributes::metric_value));

  }

  void job_cpi_impl::sync_get_description (saga::job::description & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void job_cpi_impl::sync_get_job_id (std::string & ret)
  {
    saga::attribute attr (this->proxy_);
    ret = attr.get_attribute(saga::job::attributes::jobid);
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
    instance_data data(this);
    
    saga::job::state state;
    sync_get_state(state);
    
    // If the job is not in 'New' state, we can't run it.
    if (saga::job::New != state) 
    {
      SAGA_ADAPTOR_THROW("Could not run the job: the job has already been started!",
                         saga::IncorrectState);
    }
    
    // the job already has an "official" id, since it has been registered with
    // the cream CE in the constructor.
    saga::attribute attr (this->proxy_);
    std::string creamJID = attr.get_attribute(saga::job::attributes::jobid);
    
    CreamAPI::JobIdWrapper job(creamJID, 
                               saga_to_cream2_service_url(data->rm_.get_url()),
                               std::vector<CreamAPI::JobPropertyWrapper>() );
                               
    std::vector<CreamAPI::JobIdWrapper> job_vector;
    job_vector.push_back(job);
    
    std::string leaseID = "";
    std::vector<std::string> status_vector;
    
    CreamAPI::JobFilterWrapper filter_wrapper(job_vector, status_vector, -1, -1,
                                              this->delegate_, leaseID);
                                              
    CreamAPI::ResultWrapper result;
    
    CreamAPI::AbsCreamProxy* creamClient =  
      CreamAPI::CreamProxyFactory::make_CreamProxyStart(&filter_wrapper, &result, 30); // todo: timeout
      
    if(NULL == creamClient)
    {
      SAGA_ADAPTOR_THROW("Unexpected: creamClient pointer is NULL in sync_run().", saga::NoSuccess);
    }
    
    try {
      creamClient->setCredential(this->userproxy_);
      creamClient->execute(saga_to_cream2_service_url(data->rm_.get_url()));
      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_DEBUG) {
        std::cerr << DBG_PRFX << "Successfully registerd job with: " 
                    << saga_to_cream2_service_url(data->rm_.get_url()) << std::endl; } 
    }
    catch(std::exception const & e)
    {
      SAGA_ADAPTOR_THROW("Could not register job: "+e.what(), saga::NoSuccess);
      delete creamClient;
    }  
    
    delete creamClient;
    
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

