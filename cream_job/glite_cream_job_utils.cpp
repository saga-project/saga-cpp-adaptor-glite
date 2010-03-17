//  Copyright (c) 2010 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <glite/ce/cream-client-api-c/VOMSWrapper.h>
#include <glite/ce/cream-client-api-c/CreamProxyFactory.h>

using namespace glite::ce::cream_client_api::soap_proxy;
using namespace glite::ce::cream_client_api::util;
namespace API = glite::ce::cream_client_api::soap_proxy;

#include "glite_cream_job_utils.hpp"

////////////////////////////////////////////////////////////////////////////////
//
bool glite_cream_job::try_delegate_proxy(std::string serviceAddress, 
                                         std::string delegationID,
                                         std::string localProxyPath,
                                         std::string & errorMessage)
{
  int connectionTimeout = 30; // seconds
  
  API::AbsCreamProxy * creamClient = 
    API::CreamProxyFactory::make_CreamProxyDelegate(delegationID, connectionTimeout);
  
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
std::string glite_cream_job::create_jsl_from_sjd (const saga::job::description & jd)
{
  using namespace saga::job;  
  saga::attribute attr (jd);
  
  std::stringstream oss;
  
  oss << "[ " << std::endl;
  oss << "Type = \"Job\";" << std::endl; // Only possible type
  
  // Executable
  if(attr.attribute_exists(attributes::description_executable)) {
    oss << "Executable = \"" 
        << attr.get_attribute(attributes::description_executable) << "\"|" << std::endl;
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
 
  oss << " ]" << std::endl;

  return oss.str();
}
