//  Copyright (c) 2009-2010 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include <glite/ce/cream-client-api-c/VOMSWrapper.h>
#include <glite/ce/cream-client-api-c/CreamProxyFactory.h>

using namespace glite::ce::cream_client_api::soap_proxy;
using namespace glite::ce::cream_client_api::util;
namespace CreamAPI = glite::ce::cream_client_api::soap_proxy;

#include "glite_cream_job_utils.hpp"#include <boost/algorithm/string.hpp>

////////////////////////////////////////////////////////////////////////////////
// returns true if scheme is supported, false otherwise
bool glite_cream_job::can_handle_scheme(saga::url & url)
{
  std::string scheme(url.get_scheme());

  if (scheme != "cream" && scheme !=  "https")
    return false;
  else
    return true;
}


////////////////////////////////////////////////////////////////////////////////
// returns true if the hostname is valid, false otherwise
bool glite_cream_job::can_handle_hostname(saga::url & url)
{
  std::string hostname(url.get_host());
  
  if (hostname.empty())
    return false;
  else
    return true;
}


////////////////////////////////////////////////////////////////////////////////
// 
std::string glite_cream_job::pack_delegate_and_userproxy(std::string delegate, 
                                                         std::string userproxy)
{
  std::string packed_string = delegate;
  packed_string += INTERNAL_SEP;
  packed_string += userproxy;
  
  return packed_string;
}


////////////////////////////////////////////////////////////////////////////////
// unpacks the delegate id and userproxy path from a single string
bool glite_cream_job::unpack_delegate_and_userproxy(std::string pack, 
                                                    std::string & delegate, 
                                                    std::string & userproxy)
{
  std::vector<std::string> strings;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    
  boost::char_separator<char> sep(INTERNAL_SEP);
  tokenizer tokens(pack, sep);
    
  for( tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
    strings.push_back(*tok_iter);
    
  if(strings.size() == 2)
  {
    delegate = strings[0];
    userproxy = strings[1];
  }
  else
  {
    return false;
  }
  
  return true;
}


////////////////////////////////////////////////////////////////////////////////
//
std::string glite_cream_job::saga_to_gridsite_delegation_service_url(saga::url url)
{
  saga::url gsd_service_url(url.clone());

  gsd_service_url.set_scheme("https");
  if(gsd_service_url.get_port() < 0) // todo! 
    gsd_service_url.set_port(8443);
    
  std::string service_path = "/ce-cream/services/gridsite-delegation";
  gsd_service_url.set_path(service_path);
  
  return gsd_service_url.get_url();
}


////////////////////////////////////////////////////////////////////////////////
//
std::string glite_cream_job::saga_to_cream2_service_url(saga::url url)
{
  saga::url cream2_service_url(url.clone());
 
  cream2_service_url.set_scheme("https");
  if(cream2_service_url.get_port() < 0) // todo! 
    cream2_service_url.set_port(8443);
  
  std::string service_path = "/ce-cream/services/CREAM2";
  cream2_service_url.set_path(service_path);
  
  return cream2_service_url.get_url();
}

////////////////////////////////////////////////////////////////////////////////
// 
/*std::string glite_cream_job::get_job_id_from_url(saga::url url)
{
  std::string path(url.get_path());

  std::vector<std::string> strings;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    
  boost::char_separator<char> sep("/");
  tokenizer tokens(url.get_path(), sep);
    
  for( tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
    strings.push_back(*tok_iter);
  }

  if( strings.size() > 0 )
    return strings.back();
  else
    return "";
}*/

////////////////////////////////////////////////////////////////////////////////
// 
saga::job::state glite_cream_job::cream_to_saga_job_state(std::string cream_job_state)
{
  if(     cream_job_state == "PENDING" ||
          cream_job_state == "IDLE"    ||
          cream_job_state == "RUNNING" ||
          cream_job_state == "REALLY-RUNNING")
            return saga::job::Running;
            
  else if(cream_job_state == "HELD")
            return saga::job::Suspended;
            
  else if(cream_job_state == "CANCELLED")
            return saga::job::Canceled;
            
  else if(cream_job_state == "DONE-OK")
            return saga::job::Done;
            
  else if(cream_job_state == "DONE-FAILED" ||
          cream_job_state == "ABORTED")
            return saga::job::Failed;
            
  else if(cream_job_state == "REGISTERED")
            return saga::job::New;
            
  else      return saga::job::Unknown;
}

////////////////////////////////////////////////////////////////////////////////
//
bool glite_cream_job::get_batchsystem_and_queue_from_url(std::string & batchsystem, 
                                                         std::string & queue, 
                                                         const saga::url & url)
{
  if(url.get_path().empty())
  {
    // no path - there's nothing we can do. 
    return false;
  }
  
  std::vector<std::string> strings;
  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    
  boost::char_separator<char> sep("-");
  tokenizer tokens(url.get_path(), sep);
    
  for( tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter) {
    strings.push_back(*tok_iter);
  }
  
  if(strings.size() == 3)
  {
    if(strings[0] != "cream" && strings[0] != "/cream") 
    {
      // path is malformated - doesn't start with "cream-"
      return false;
    }
    else
    {
      batchsystem = strings[1];
      queue       = strings[2];
    }
  }
  else
  {
    // path is malformated - can't be split up into three components 
    return false;
  }
  
  return true;
}


////////////////////////////////////////////////////////////////////////////////
//
bool glite_cream_job::try_delegate_proxy(std::string serviceAddress, 
                                         std::string delegationID,
                                         std::string localProxyPath,
                                         std::string & errorMessage)
{
  int connectionTimeout = 30; // seconds
  
  CreamAPI::AbsCreamProxy * creamClient = 
    CreamAPI::CreamProxyFactory::make_CreamProxyDelegate(delegationID, connectionTimeout);
  
  if(localProxyPath.empty())
  {
    errorMessage = std::string("Unexpected: localProxyPath can't be empty.");
    return false;
  }
 
  if(NULL == creamClient)
  {
    errorMessage =  std::string("Unexpected: creamClient pointer is NULL.");
    return false;
  }
  
  try 
  {
    creamClient->setCredential(localProxyPath);
    creamClient->execute(serviceAddress);

  } 
  catch(std::exception const & e) 
  {
    errorMessage = e.what();
    delete creamClient;
    return false;
  }
  
  delete creamClient;
  
  return true;   
}

////////////////////////////////////////////////////////////////////////////////
//
bool glite_cream_job::start_job_has_failed(CreamAPI::ResultWrapper const & rw, 
                                           std::string & jid, std::string & why)
{
  std::list< std::pair< JobIdWrapper, std::string > > target;
  std::list< std::pair< JobIdWrapper, std::string > >::const_iterator c_it;
  
  // returns a list of job we tried to start but they where not present in the 
  // CE CREAM
  rw.getNotExistingJobs(target);
  c_it = target.begin();   
  while(c_it != target.end())
  {
    jid = c_it->first.getCreamJobID();
    why = c_it->second;
    return true;
    c_it++;
  }
  
  // returns the list of job that are not in no one of the states specified in 
  // the statusVec
  rw.getNotMatchingStatusJobs(target);
  c_it = target.begin();   
  while(c_it != target.end())
  {
    jid = c_it->first.getCreamJobID();
    why = c_it->second;
    return true;
    c_it++;
  }

  // returns the list of job that are not in the timerange fromDate - toDate
  rw.getNotMatchingDateJobs(target);
  c_it = target.begin();   
  while(c_it != target.end())
  {
    jid = c_it->first.getCreamJobID();
    why = c_it->second;
    return true;
    c_it++;
  }
  
  // returns the list of job that have not registered with the delegation
  // identifier specified in the delegationID variable
  rw.getNotMatchingProxyDelegationIdJobs(target);
  c_it = target.begin();   
  while(c_it != target.end())
  {
    jid = c_it->first.getCreamJobID();
    why = c_it->second;
    return true;
    c_it++;
  }
  
  // returns the list of job that have not registered with the lease identifier 
  // specified in the leaseID variable (but the case it is empty)
  rw.getNotMatchingLeaseIdJobs(target);
  c_it = target.begin();   
  while(c_it != target.end())
  {
    jid = c_it->first.getCreamJobID();
    why = c_it->second;
    return true;
    c_it++;
  }
  
  return false;
}

////////////////////////////////////////////////////////////////////////////////
//
std::string glite_cream_job::create_jsl_from_sjd (const saga::job::description & jd, 
                                                  const saga::url & url)
{
  using namespace saga::job;  
  saga::attribute attr (jd);
  
  std::stringstream oss;
  
  oss << "[ " << std::endl;
  oss << "Type = \"Job\";" << std::endl; // Only possible type
  
  // Executable
  if(attr.attribute_exists(attributes::description_executable)) {
    oss << "Executable = \"" 
        << attr.get_attribute(attributes::description_executable) << "\";" << std::endl;
  }
  
  // Arguments
  if (jd.attribute_exists(attributes::description_arguments)) {
    std::vector<std::string> arguments = 
    jd.get_vector_attribute(attributes::description_arguments);
        
    if( arguments.size() > 0 ) 
    {
       oss << "Arguments = \"";
            
       std::vector<std::string>::iterator end = arguments.end();
       for (std::vector<std::string>::iterator it = arguments.begin(); it != end; ++it)
       {
         oss << (*it) << " ";
       } 
            
       oss << "\";" << std::endl;
    }
  }
  
  // Environment
  if (jd.attribute_exists(attributes::description_environment)) {
    std::vector<std::string> environment = 
    jd.get_vector_attribute(attributes::description_environment);
        
    if( environment.size() > 0 ) 
    {
       oss << "Environment = {";
            
       std::vector<std::string>::iterator end = environment.end();
       for (std::vector<std::string>::iterator it = environment.begin(); it != end; ++it)
       {
         oss << "\"" << (*it) << "\"";
         if(it != end) oss << ",";
       } 
            
       oss << "};" << std::endl;
    }
  }

  // StdInput
  if(attr.attribute_exists(attributes::description_input)) {
    oss << "StdInput = \"" 
        << attr.get_attribute(attributes::description_input) << "\";" << std::endl;
  }

  // StdOutput
  if(attr.attribute_exists(attributes::description_output)) {
    oss << "StdOutput = \"" 
        << attr.get_attribute(attributes::description_output) << "\";" << std::endl;
  }

  // StdError
  if(attr.attribute_exists(attributes::description_error)) {
    oss << "StdError = \"" 
        << attr.get_attribute(attributes::description_error) << "\";" << std::endl;
  }

  // Queue
  if(attr.attribute_exists(attributes::description_queue)) {
    oss << "QueueName = \"" 

        << attr.get_attribute(attributes::description_queue) << "\";" << std::endl;
  }
  
  // CpuNumber
  if(attr.attribute_exists(attributes::description_number_of_processes)) {
    oss << "CpuNumber = \"" 
        << attr.get_attribute(attributes::description_number_of_processes) << "\";" << std::endl;
  }
  
  // VirtualOrganisation
  // We only use the first entry - this shouldn't be a vector type anyways
  if (jd.attribute_exists(attributes::description_job_project)) {
    std::vector<std::string> project_names = 
    jd.get_vector_attribute(attributes::description_job_project);
        
    if( project_names.size() > 0 ) 
    {
      oss << "VirtualOrganisation = \"" 
          << project_names[0] << "\";" << std::endl;
    }
  }   
 
  // we adopt the behavior from cream-pbs-cream_A - URLs can define like this:
  // <host>[:<port>]/cream-<lrms-system-name>-<queue-name>. This allows us to
  // encode BatchSystem and Queue in the URL. If they're not explicitly defined
  // in the jd, we'll try to extract them from the rm url. 
  std::string batchsystem, queue;
  bool success = get_batchsystem_and_queue_from_url(batchsystem, queue, url);
  if(success) 
  {
    oss << "QueueName = \"" << queue << "\";" << std::endl;
    oss << "BatchSystem = \"" << batchsystem << "\";" << std::endl;
    
    SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
      std::cerr << DBG_PRFX << "Successfully extracted BatchSystem (" << batchsystem 
                << ") and Queue (" << queue << ") attributes from URL." << std::endl; }
  }
  
 
  oss << " ]" << std::endl;

  return oss.str();
}
