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



