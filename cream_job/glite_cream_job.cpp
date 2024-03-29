//  Copyright (c) 2009-2010 Ole Weidner (oweidner@cct.lsu.edu)
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

  ///////////////////////////////////////////////////////////////////////
  //
  std::string job_cpi_impl::get_job_id_priv_()
  {
    saga::adaptors::attribute attr (this);
    return attr.get_attribute (saga::job::attributes::jobid);
  }
      
  ///////////////////////////////////////////////////////////////////////
  //
  void job_cpi_impl::set_job_id_priv_(std::string jobid)
  {
    saga::adaptors::attribute attr (this);
    attr.set_attribute (saga::job::attributes::jobid, jobid);
  }

  ///////////////////////////////////////////////////////////////////////
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
    this->update_state_priv_(saga::job::Unknown);
    
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
            strm << "For job submission, batchsystem and queue name need to be encoded in the url path: " 
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
      
      this->job_description_wrapper_ = new CreamAPI::JobDescriptionWrapper(jdl, 
        this->delegate_, leaseID, delegationProxy, autostart, internal_jobid_);
      
      CreamAPI::AbsCreamProxy::RegisterArrayRequest reqs;
      reqs.push_back(this->job_description_wrapper_);
      CreamAPI::AbsCreamProxy::RegisterArrayResult resp;
      
      int connection_timeout = 30;
      
      CreamAPI::AbsCreamProxy* creamClient = 
        CreamAPI::CreamProxyFactory::make_CreamProxyRegister(&reqs, &resp, connection_timeout);
      THROW_IF_NULL(creamClient, "job c'tor");

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
        
      //if(CreamAPI::JobIdWrapper::OK != registrationResponse.get<0>())
      if(CreamAPI::JobIdWrapper::OK != boost::get<0>(registrationResponse))
      {
        SAGA_ADAPTOR_THROW("Could not register job: "+registrationResponse.get<2>(), saga::NoSuccess);
        delete creamClient;
      }
      else
      {
        this->cream_url_ = boost::get<1>(registrationResponse).getCreamURL();
        this->cream_job_id_ = boost::get<1>(registrationResponse).getCreamJobID();
        
        saga::url saga_jobid(this->cream_url_);
        saga_jobid.set_path("/"+this->cream_job_id_);
        
        this->update_state_priv_(saga::job::New);
        this->set_job_id_priv_(saga_jobid.get_url());

        delete creamClient;
      }
      
      
    } // init from jd
  
  }

  //////////////////////////////////////////////////////////////////////////////
  // destructor
  job_cpi_impl::~job_cpi_impl (void)
  {
    if(NULL != job_description_wrapper_)
      delete job_description_wrapper_;
  }


  //////////////////////////////////////////////////////////////////////////////
  //  SAGA API functions
  void job_cpi_impl::sync_get_state (saga::job::state & ret)
  {
    instance_data data(this);
    
    // get the current ('old') state
    saga::monitorable monitor (this->proxy_);
    saga::metric m (monitor.get_metric(saga::metrics::task_state));
    saga::job::state old_state = saga::adaptors::
      job_state_value_to_enum(m.get_attribute(saga::attributes::metric_value));
      
    saga::job::state new_state = old_state;
    
    // get our jobid
    saga::attribute attr (this->proxy_);
    std::string jobid = attr.get_attribute(saga::job::attributes::jobid);
    
    CreamAPI::JobIdWrapper job(this->cream_job_id_, 
                               saga_to_cream2_service_url(data->rm_.get_url()),
                               std::vector<CreamAPI::JobPropertyWrapper>() );
                               
    std::vector<CreamAPI::JobIdWrapper> job_vector;
    job_vector.push_back(job);
    
    std::string leaseID = "";
    std::vector<std::string> status_vector;
    
    CreamAPI::JobFilterWrapper filter_wrapper(job_vector, status_vector, -1, -1,
                                              this->delegate_, leaseID);
                                              
    CreamAPI::ResultWrapper result;
    CreamAPI::AbsCreamProxy::StatusArrayResult status;
    
    CreamAPI::AbsCreamProxy* creamClient =  
      CreamAPI::CreamProxyFactory::make_CreamProxyStatus(&filter_wrapper, &status, 30); // todo: timeout
    THROW_IF_NULL(creamClient, "sync_get_state");
    
    try {
      creamClient->setCredential(this->userproxy_);
      creamClient->execute(saga_to_cream2_service_url(data->rm_.get_url()));
    }
    catch(std::exception const & e)
    {
      SAGA_ADAPTOR_THROW("Could not query job status: "+e.what(), saga::NoSuccess);
      delete creamClient;
    } 
    
    //std::map<std::string, boost::tuple<CreamAPI::JobStatusWrapper::RESULT, CreamAPI::
    //     JobStatusWrapper, std::string> >::const_iterator jobIt = status.begin();
    CreamAPI::AbsCreamProxy::StatusArrayResult::const_iterator job_it = status.begin();
  
    while( job_it != status.end() ) 
    {
      if( job_it->second.get<0>() == CreamAPI::JobStatusWrapper::OK ) 
      {
        new_state = cream_to_saga_job_state(job_it->second.get<1>().getStatusName());
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_DEBUG) {
        std::cerr << DBG_PRFX << "Successfully querried job state for job id " << this->cream_job_id_ 
                  << ": " << job_it->second.get<1>().getStatusName() << std::endl; } 
        ++job_it;
      }
      else
      {
        SAGA_ADAPTOR_THROW("Could not query status for job id "+ this->cream_job_id_ 
                           + "because: " + job_it->second.get<2>(), saga::NoSuccess);
      }
    }
  
    delete creamClient;
  
    if(old_state != new_state)
      this->update_state_priv_(new_state);
      
    ret = new_state;
  }

  void job_cpi_impl::sync_get_description (saga::job::description & ret)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }

  void job_cpi_impl::sync_get_job_id (std::string & ret)
  {
    ret = this->get_job_id_priv_();
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


  //////////////////////////////////////////////////////////////////////////////
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
    //std::string creamJID = attr.get_attribute(saga::job::attributes::jobid);
    
    //std::cout << "starting: " << creamJID << std::endl;
    
    CreamAPI::JobIdWrapper job(this->cream_job_id_, 
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
    THROW_IF_NULL(creamClient, "sync_run"); 
    
    try {
      creamClient->setCredential(this->userproxy_);
      creamClient->execute(saga_to_cream2_service_url(data->rm_.get_url()));
    }
    catch(std::exception const & e)
    {
      SAGA_ADAPTOR_THROW("Could not start job: "+e.what(), saga::NoSuccess);
      delete creamClient;
    }  
    
    std::string error_message, jid;
    if(start_job_has_failed(result, jid, error_message))
    {
      SAGA_ADAPTOR_THROW("Could not start job "+jid+": "+error_message, saga::NoSuccess);
    }
    else
    {
      SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_DEBUG) {
        std::cerr << DBG_PRFX << "Successfully started job with: " 
                  << saga_to_cream2_service_url(data->rm_.get_url()) << std::endl; } 
    }
    
    delete creamClient;
    
  }

  void job_cpi_impl::sync_cancel (saga::impl::void_t & ret, 
                                  double timeout)
  {
    SAGA_ADAPTOR_THROW ("Not Implemented", saga::NotImplemented);
  }


  /////////////////////////////////////////////////////////////////////
  //
  void job_cpi_impl::sync_wait (bool   & ret, 
                                double   timeout)
  {  
    std::string job_id = this->get_job_id_priv_();
    double wait_count = 0.0;
    ret = false;
   
    try {
      saga::job::state job_state; this->sync_get_state(job_state);
    
      if(job_state == saga::job::New ||
         job_state == saga::job::Done ||
         job_state == saga::job::Failed ||
         job_state == saga::job::Canceled) 
      {
      
        std::string state_name = saga::job::detail::get_state_name(job_state);
        SAGA_OSSTREAM strm;
        strm << "Could not wait for job " << job_id << ": " 
             << "job is already in '" << state_name << "' state."; 
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::IncorrectState);   
      }
      
      if(timeout < 0.0) 
      {
        saga::job::state s; this->sync_get_state(s);
        while(s == saga::job::Running || s == saga::job::Suspended) 
        {
          this->sync_get_state(s);
           sleep(1);
        }
        ret = true;
      }
          
      else if(timeout > 0.0) 
      {
        while(wait_count <= timeout) 
        {
          saga::job::state s; this->sync_get_state(s);
          if(s != saga::job::Running && s != saga::job::Suspended) 
          {
            ret = true;
            break;
          }
          wait_count += 1.0; sleep(1); 
        }
      }
      
      else 
      {
        saga::job::state s; this->sync_get_state(s);
        if(s != saga::job::Running) 
        {
          ret = true;
        }
      }
    }
    catch(saga::exception const & e)
    {
       //catch exceptions from other methods
       SAGA_OSSTREAM strm;
       strm << "Could not wait for job " << job_id << ": " 
            << e.get_message();
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), e.get_error()); 
    }

  }
  
  
  /////////////////////////////////////////////////////////////////////
  // utility functions, etc... 
  void job_cpi_impl::update_state_priv_(saga::job::state newstate)
  {
    saga::monitorable monitor (this->proxy_);
    saga::adaptors::metric m (monitor.get_metric(saga::metrics::task_state));
    m.set_attribute(saga::attributes::metric_value, 
                    saga::adaptors::job_state_enum_to_value(newstate));
  }


} // namespace glite_cream_job
////////////////////////////////////////////////////////////////////////

