//  Copyright (c) 2009-2010 Ole Weidner (oweidner@cct.lsu.edu)
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
#include <glite/ce/cream-client-api-c/CreamProxyFactory.h>
#include <glite/ce/cream-client-api-c/JobDescriptionWrapper.h>
using namespace glite::ce::cream_client_api::soap_proxy;
using namespace glite::ce::cream_client_api::util;
namespace CreamAPI = glite::ce::cream_client_api::soap_proxy;

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
    : base_cpi (p, info, adaptor, cpi::Noflags), delegation_("")
  {
    instance_data data(this);
    
    // check if we can handle scheme
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
      // try to delegate all (?) valid proxies to the resource manager.
      for(unsigned int i = 0; i < context_list.size(); i++)
      {
        std::string errorMessage = "";
        this->userproxy_ = context_list[i].get_attribute(saga::attributes::context_userproxy);
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

    // A job description needs at least an 'Executable' 
    // attribute. Doesn't make sense without one.
    if (!jd.attribute_exists(saga::job::attributes::description_executable) ||
        jd.get_attribute(saga::job::attributes::description_executable).empty())
    {
      SAGA_OSSTREAM strm;
		  strm << "Could not create a job object for " << data->rm_ << ". " 
           << "The job description is missing the mandatory 'executable' attribute.";
		  SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::BadParameter); 
    }
    
    /////////////////////////////
    // DELEGATE PROXY
    //
    // delegate the x509 userproxy to the cream ce service if this is the first
    // time either this object's create_job or get_job method is called.
    // delegating is a VERY expensive operation and delegation is not required
    // for any of the job::service methods, like list(), etc...
    if(this->delegation_.length() < 1)
    {    
      // create a unique random delegation ID
      std::string error_message("");
      std::string del_id(saga::uuid().string());
      
      bool success = try_delegate_proxy(saga_to_gridsite_delegation_service_url(data->rm_), 
                                        del_id, this->userproxy_, error_message);                                 
      if(!success)
      {
        SAGA_OSSTREAM strm;
        strm << "Could not delegate (id="<< this->delegation_ <<") userproxy " << this->userproxy_ << " to " 
             << saga_to_gridsite_delegation_service_url(data->rm_) << ": " << error_message;
        SAGA_ADAPTOR_THROW(SAGA_OSSTREAM_GETSTRING(strm), saga::AuthorizationFailed);
      }
      else
      {          
        this->delegation_ = del_id;
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
        std::cerr << DBG_PRFX << "Successfully delegated userproxy " << this->userproxy_ 
                  << " to " << saga_to_gridsite_delegation_service_url(data->rm_) 
                  << " with id " << this->delegation_ << "." << std::endl; }
      }
    }   
    // DELEGATE PROXY
    /////////////////////////////
    
    // we're going to abuse the JobContact attribute to smuggle the 
    // delegation ID into the job instance.     
    std::string packed_str = pack_delegate_and_userproxy(this->delegation_,
                                                         this->userproxy_);

    jd.set_attribute(saga::job::attributes::description_job_contact, packed_str);
    
    
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


  //////////////////////////////////////////////////////////////////////////////
  //
  void job_service_cpi_impl::sync_list (std::vector <std::string> & ret)
  {
    instance_data data(this);   
      
    std::vector<CreamAPI::JobIdWrapper> jid_wrapper_v;
    
    CreamAPI::AbsCreamProxy* creamClient =  
      CreamAPI::CreamProxyFactory::make_CreamProxyList(&jid_wrapper_v, 30); // todo: timeout
    THROW_IF_NULL(creamClient, "sync_list");
    
    try {
      creamClient->setCredential(this->userproxy_);
      creamClient->execute(saga_to_cream2_service_url(data->rm_.get_url()));
    }
    catch(std::exception const & e)
    {
      SAGA_ADAPTOR_THROW("Could not get a list of jobs: "+e.what(), saga::NoSuccess);
      delete creamClient;
    }  
    
    std::vector<CreamAPI::JobIdWrapper>::const_iterator job_it = jid_wrapper_v.begin();
    while(job_it != jid_wrapper_v.end()) {
      ret.push_back(job_it->getCreamURL() + "/" +job_it->getCreamJobID());
      ++job_it;
    }
    
    delete creamClient;
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

