//  Copyright (c) 2009-2010 Ole Weidner (oweidner@cct.lsu.edu)
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


#define DBG_PRFX     "gLite Cream Adaptor: "
#define INTERNAL_SEP "[:::]"

namespace glite_cream_job
{
////////////////////////////////////////////////////////////////////////////////
// returns true if scheme is supported, false otherwise
bool can_handle_scheme(saga::url & url);

////////////////////////////////////////////////////////////////////////////////
// returns true if the hostname is valid, false otherwise
bool can_handle_hostname(saga::url &url);

////////////////////////////////////////////////////////////////////////////////
// packs the delegate id and userproxy path into one string
std::string pack_delegate_and_userproxy(std::string delegate, std::string userproxy);

////////////////////////////////////////////////////////////////////////////////
// unpacks the delegate id and userproxy path from a single string. returns
// true on success, false otherwise.
bool unpack_delegate_and_userproxy(std::string pack, std::string & delegate, std::string & userproxy);

////////////////////////////////////////////////////////////////////////////////
// converts a saga url to a CREAM2 service address
std::string saga_to_cream2_service_url(saga::url url);

////////////////////////////////////////////////////////////////////////////////
// converts a saga url to a gridsite delegation service address
std::string saga_to_gridsite_delegation_service_url(saga::url url);

////////////////////////////////////////////////////////////////////////////////
// extracts the job id from a a give cream url (https://.../)
//std::string get_job_id_from_url(saga::url url);

////////////////////////////////////////////////////////////////////////////////
// tries to extract batchsystem name and queue name from an url path. returns
// true on success, false otherwise
bool get_batchsystem_and_queue_from_url(std::string & batchsystem, 
                                        std::string & queue, const saga::url & url);
////////////////////////////////////////////////////////////////////////////////
// translates CREAM Job states to saga::job::states
saga::job::state cream_to_saga_job_state(std::string cream_job_state);

////////////////////////////////////////////////////////////////////////////////
// returns true if 'rw' indicates an error. in this case 'why' will contain
// the reason and jid the CREAM job id. returns false otherwise. 
bool start_job_has_failed(CreamAPI::ResultWrapper const & rw, std::string & jid, std::string & why);

////////////////////////////////////////////////////////////////////////////////
//
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
          std::cerr << DBG_PRFX << "X.509 context found pointing to user proxy at " 
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
            std::cerr << DBG_PRFX << "Coundl't open proxyfile: " 
                      << V.getErrorMessage() << std::endl;
            }
	        }
          else {
            error += V.getErrorMessage();
            context_error_list.push_back(error);
            SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
              std::cerr << DBG_PRFX << "Error while reading proxyfile: " 
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
              std::cerr << DBG_PRFX << "Certificate is expired. Please check. " 
                        << std::endl;
            }
          }
          else
          {
            // SUCCESS
            SAGA_VERBOSE(SAGA_VERBOSE_LEVEL_INFO) {
              std::cerr << DBG_PRFX << "Certificate seems to be valid and can be used." 
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
                        
// creates a cream jsl from a saga job description. 
std::string create_jsl_from_sjd (const saga::job::description & jd, const saga::url & url);


} // namespace

#endif //ADAPTORS_X509_CONTEXT_ADAPTOR_UTILS_HPP
