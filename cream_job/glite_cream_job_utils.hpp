//  Copyright (c) 2009 Ole Weidner (oweidner@cct.lsu.edu)
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADAPTORS_X509_CONTEXT_ADAPTOR_UTILS_HPP
#define ADAPTORS_X509_CONTEXT_ADAPTOR_UTILS_HPP

#include <saga/saga.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <glite/ce/cream-client-api-c/VOMSWrapper.h>

using namespace glite::ce::cream_client_api::soap_proxy;
using namespace glite::ce::cream_client_api::util;


#define DBG_PRFX    "### GLITE CREAM ADAPTOR ### "

namespace glite_cream_job
{

inline void check_x509_voms_cert(saga::context & context,
                                 std::vector<saga::context> & context_list,
                                 std::vector<std::string> & context_error_list)
{
      std::string error;
      
      if (context.attribute_exists (saga::attributes::context_type) &&
          context.get_attribute (saga::attributes::context_type) == "x509")
      {
        if(!context.attribute_exists (saga::attributes::context_userproxy)) {
          error += "x.509 user proxy attribute not set.";
          context_error_list.push_back(error);
          // continue;
          return;
        }
        
        std::string userproxy(context.get_attribute (saga::attributes::context_userproxy));
        error += "x.509 user proxy @ " + userproxy + ": ";
        SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_DEBUG) {
          std::cerr << "gLite CREAM Adaptor: X.509 context found pointing to user proxy at " 
                    << userproxy << std::endl;
        }
        
        // Using boost::filesystem is much faster then using the VOMSWrapper.
        // In case the file doesn't exist, we don't want to wast anymore time.
        if(!boost::filesystem::exists(userproxy)) {
            error += "No such file or directory";
            context_error_list.push_back(error);
            // continue;
            return;
        }

        VOMSWrapper V( userproxy );
        if( !V.IsValid( ) ) {
          if(V.getErrorNum() == VOMSWrapper::FILE_NOT_FOUND) {
            error += V.getErrorMessage();
            context_error_list.push_back(error);
            SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
            std::cerr << "gLite CREAM Adaptor: Coundl't open proxyfile: " 
                      << V.getErrorMessage() << std::endl;
            }
	        }
          else {
            error += V.getErrorMessage();
            context_error_list.push_back(error);
            SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
              std::cerr << "gLite CREAM Adaptor: Error while reading proxyfile: " 
                        << V.getErrorMessage() << std::endl;
            }
          }
        }
        else {
          long int leftcert = V.getProxyTimeEnd( ) - time( NULL );
          if(leftcert <= 0) {
            error += "Certificate is expired. Please check.";
            context_error_list.push_back(error);
            SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
              std::cerr << "gLite CREAM Adaptor: Certificate is expired. Please check. " 
                        << std::endl;
            }
          }
          else
          {
            // SUCCESS
            SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
              std::cerr << "gLite CREAM Adaptor: Certificate is VALID and can be used." 
                        << std::endl;
            }
            context_list.push_back (context);
          }
        } 
      } 
};

// tries to delegate a proxy to a remote cream service. returns 'true' on
// success and 'false' on error. 'errorMessage' will contain the error
// message in case of 'false'.  
bool try_delegate_proxy(std::string serviceAddress, std::string delegationID,
                        std::string localProxyPath, std::string & errorMessage);
                        
std::string create_jsl_from_sjd (const saga::job::description & jd);


} // namespace

#endif //ADAPTORS_X509_CONTEXT_ADAPTOR_UTILS_HPP
