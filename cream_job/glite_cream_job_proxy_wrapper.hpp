//  Copyright (c) 2009 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_GLITE_CREAM_JOB_PROXY_WRAPPER_HPP
#define ADAPTORS_GLITE_CREAM_JOB_PROXY_WRAPPER_HPP

#include <glite/ce/cream-client-api-c/CreamProxyFactory.h>
#include <glite/ce/cream-client-api-c/JobFilterWrapper.h>
#include <glite/ce/cream-client-api-c/ResultWrapper.h>
#include <glite/ce/cream-client-api-c/ConfigurationManager.h>
#include <glite/ce/cream-client-api-c/creamApiLogger.h>
#include <glite/ce/cream-client-api-c/VOMSWrapper.h>
#include <glite/ce/cream-client-api-c/certUtil.h>
#include <glite/ce/cream-client-api-c/CEUrl.h>

using namespace glite::ce::cream_client_api::soap_proxy;
using namespace glite::ce::cream_client_api::util;
using namespace glite::ce::cream_client_api;
using namespace glite::ce::cream_client_api::cream_exceptions;

////////////////////////////////////////////////////////////////////////
namespace glite_cream_job
{
  class proxy_wrapper {
  
    private: 
      AbsCreamProxy *creamClient_;
  };
  
}

#endif //ADAPTORS_GLITE_CREAM_JOB_PROXY_WRAPPER_HPP
