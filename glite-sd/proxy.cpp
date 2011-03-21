/*
 * Copyright (c) Members of the EGEE Collaboration. 2009-2010.
 * See http://www.eu-egee.org/partners/ for details on the copyright
 * holders.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <openssl/ssl.h>
#include <openssl/x509.h>

#include "boost/filesystem/path.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/regex.hpp"
#include <boost/thread/thread.hpp>
#include <boost/thread/once.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "proxy_funcs.hpp"

#include <saga/packages/sd/service_description.hpp>

#ifdef WIN32
int getuid()
{
   return 0;
}
#else
#include "unistd.h"
#endif

namespace
{
   const char* X509_USER_PROXY_FILE_ENV = "X509_USER_PROXY";
   const char* X509_USER_PROXY_FILE = "x509up_u";
   const char* acseq = "1.3.6.1.4.1.8005.100.100.5";
   const char* fqan = "1.3.6.1.4.1.8005.100.100.4";

   X509* GetX509Certificate(const std::string& proxy_path,
                            std::string& error_str);

   bool ParseAsn1Output(const std::string& asn1_output,
                        const unsigned char* const asn1_data,
                        std::vector<std::string>& vos,
                        std::vector<std::string>& fqans,
                        std::string& error_str);
   void Init();
}

namespace glite_adaptor
{
boost::mutex InstanceMutex;

//The singleton instance
proxy_funcs& proxy_funcs::Instance()
{
   //Only allow one thread at a time in this code
   //as it can change the state of the singleton
   boost::mutex::scoped_lock lock(InstanceMutex);

   static proxy_funcs _singleton;
   return _singleton;
}

//Get the filename for the X509 proxy certificate
std::string proxy_funcs::GetProxyPath()
{
   std::ostringstream proxy_filename;
   const char* proxy_env = saga::detail::safe_getenv(X509_USER_PROXY_FILE_ENV);

   if ( proxy_env != NULL )
   {
      proxy_filename << proxy_env;
   }

   else
   {
      //Use the default filename of our proxy certificate
#ifdef WIN32
      proxy_filename << "C:\\tmp\\"
#else
      proxy_filename << "/tmp/"
#endif
                     << X509_USER_PROXY_FILE
                     << getuid();
   }
   return proxy_filename.str();
}

boost::once_flag ssl_once = BOOST_ONCE_INIT;
boost::mutex GPAMutex;

//Get the proxy certificate from /tmp/<x509_uid>u
////if it exists and extract useful information from it.
bool proxy_funcs::GetProxyAttributes(const std::string& proxy_cert_path,
                                     std::string& identity,
                                     std::vector<std::string>& vo,
                                     std::vector<std::string>& fqan,
                                     std::string& error_str)
{
   //Only need to call SSL_library_init() once.
   boost::call_once(&Init, ssl_once);

   //Only allow one thread at a time in this code
   //as it can change the state of the singleton
   boost::mutex::scoped_lock lock(GPAMutex);

   OBJ_create(acseq, "acseq", "acseq");

   //Clear out our output variables
   identity = "";
   vo.clear();
   fqan.clear();
   error_str = "";

   boost::filesystem::path proxy_path;

   if ( proxy_cert_path.empty() )
   {
      proxy_path = GetProxyPath();
   }

   else
   {
      proxy_path = proxy_cert_path;
   }

   std::time_t modified_time;

   if ( boost::filesystem::exists(proxy_path) )
   {
      modified_time = boost::filesystem::last_write_time(proxy_path);
   }

   //Do we need to do anything?
   if ( (_path != proxy_path) ||
        (_modified_time != modified_time) ||
        (_modified_time == 0) )
   {
      X509* x509 = NULL;
      x509 = GetX509Certificate(proxy_path.string(), error_str);

      //Report failure
      if ( x509 == NULL )
      {
         return false;
      }

      //We've read the certificate in.
      //Now get the identity from this certificate
      std::string x509_identity_str = "Unknown";
      X509_NAME* x509_name = X509_get_issuer_name(x509);

      if ( x509_name == NULL )
      {
         error_str = "Unable to get identity from certificate";
         return false;
      }

      else
      {
         //Get the identity in a human readable form
         char* x509_identity_ptr =  X509_NAME_oneline(x509_name, NULL, 0);

         //Copy it into something more useful
         x509_identity_str = x509_identity_ptr;

         //Free our allocated memory
         OPENSSL_free(x509_identity_ptr);

         identity = x509_identity_str;
      }

      //Now look for an extension
      int nid = OBJ_txt2nid("acseq");

      X509_EXTENSION* x509_ext = NULL;

      int index = X509_get_ext_by_NID(x509, nid, -1);

      if ( index < 0 )
      {
         error_str = "Couldn't find extension in certificate";
         return false;
      }

      x509_ext = X509_get_ext(x509, index);

      if ( x509_ext == NULL )
      {
         error_str = "Couldn't get extension from certificate";
         X509_free(x509);
         return false;
      }

      //We have the extension, now get it in ASN.1 form
      ASN1_OCTET_STRING* asn = NULL;
      asn = X509_EXTENSION_get_data(x509_ext);

      if ( asn == NULL )
      {
         error_str = "Couldn't get extension in ASN.1 form";
         X509_free(x509);
         return false;
      }

      //If we are here then we have the certificate extension
      //that we were after in ASN.1 form

      //We're going to make a copy of the ASN.1 data to play with
      //Add room for a terminating '\0'
      int asn1_data_len = asn->length + 1;
      unsigned char* asn1_data = new unsigned char[asn1_data_len];
      (void)memcpy(asn1_data, asn->data, asn1_data_len);

      //Just to be safe add a '\0' to the end
      //we made room for it earlier
      asn1_data[asn1_data_len - 1] = 0;

      //Free up the X509 certificate
      //that we've finished with
      X509_free(x509);
      x509 = NULL;
      x509_ext = NULL;
      asn = NULL;

      //Create a memory BIO to store the results
      //of our ASN.1 string parse
      BIO* bio_mem = BIO_new(BIO_s_mem());

      if ( bio_mem == NULL )
      {
         error_str = "Unable to create a memory BIO";
         return false;
      }

      //Parse our ASN.1 string
      ASN1_parse(bio_mem, asn1_data, asn1_data_len, 0);
      char* bio_data_ptr = NULL;
      int bio_data_len = BIO_get_mem_data(bio_mem, &bio_data_ptr);

      //Copy the results into a string for ease of use
      std::string output(bio_data_ptr, bio_data_len);

      //Free up all allocated items that we've finished with
      BIO_free(bio_mem);
      bio_mem = NULL;
      bio_data_ptr = NULL;

      //Now finally get the data from the
      //parsed and unparsed ASN.1 representation
      //of our proxy certificate
      if ( ParseAsn1Output(output, asn1_data, vo, fqan, error_str) == false )
      {
         error_str = "No VO/FQANs found";
         delete [] asn1_data;
         asn1_data = NULL;
         return false;
      }

      //Delete our allocated data buffer
      //now that we've finished with it
      delete [] asn1_data;
      asn1_data = NULL;

      //If we're here then cache our results
      _path = proxy_path;
      _modified_time = modified_time;
      _identity = identity;
      _vo = vo;
      _fqan = fqan;

   }

   else
   {
      identity = _identity;
      vo = _vo;
      fqan = _fqan;
   }

   error_str = "";
   return true;
}
} //namespace

namespace
{
void Init()
{
   SSL_library_init();
}

X509* GetX509Certificate(const std::string& proxy_path,
                         std::string& error_str) 
{
   X509* x509_ret_val = NULL;

   //Creat a new file BIO to read the certificate in with
   BIO* cert_in_bio = BIO_new(BIO_s_file());

   //Open up the proxy certificate and read it in
   if ( cert_in_bio )
   {
      if  ( BIO_read_filename(cert_in_bio,
                              proxy_path.c_str() ) > 0 )
      {
         x509_ret_val = PEM_read_bio_X509(cert_in_bio, NULL, 0, NULL);
         BIO_free(cert_in_bio);

         if ( x509_ret_val == NULL )
         {
            error_str = "Couldn't find a valid proxy";
            return NULL;
         }

         //Check the dates
         //We'll ignore any errors in trying to get the
         //NotBefore and NotAfter times and assume they're OK
         else
         {
            time_t t;
            t = time(NULL);
            int i;

            //Check that the NotBefore data is OK
            i = X509_cmp_time(X509_get_notBefore(x509_ret_val), &t);
            if ( i > 0 )
            {
               //We're before the NotBefore time
               //Delete the X509 certificate and return error
               X509_free(x509_ret_val);
               x509_ret_val = NULL;
               error_str = "Certificate not yet valid";
            }

#if SD_DEBUG
            std::cout << "X509_get_notBefore() returned " << i <<std::endl;
#endif

            //Check that the NotBefore data is OK
            i = X509_cmp_time(X509_get_notAfter(x509_ret_val), &t);
            if ( i < 0 )
            {
               //We're at or after the NotAfter time
               //Delete the X509 certificate and return error
               X509_free(x509_ret_val);
               x509_ret_val = NULL;
               error_str = "Certificate expired";
            }

#if SD_DEBUG
            std::cout << "X509_get_notAfter() returned " << i <<std::endl;
#endif
         }
      }

      else
      {
         error_str =  "Unable to read certificate at " + proxy_path;
         BIO_free(cert_in_bio);
         return NULL;
      }
   }

   else
   {
      error_str = "Couldn't create BIO for reading " + proxy_path;
      return NULL;
   }
   return x509_ret_val;
}

//We are going to get the VO and FQANs
//from the certificate extension in a rather roundabout fashion
//This is because a proper extension parser is difficult and
//complete overkill for our purposes
//We know the format of the extension so look for
//our object of choice in it, 1.3.6.1.4.1.8005.100.100.4
//Then we get the vo and the FQANs.
//The VO is tricky as the ASN.1 parser doesn't decode it by default
//We have to extract the relevant part of the ASN.1 string by hand
//and parse is seperately.
bool ParseAsn1Output(const std::string& asn1_output,
                     const unsigned char* const asn1_data,
                     std::vector<std::string>& vos,
                     std::vector<std::string>& fqans,
                     std::string& error_str)
{
   //Clean out our two output vectors
   vos.clear();
   fqans.clear();

   //After the Object Identifier there is a SEQUENCE, the VO,
   //another SEQUENCE, the FQANs then another SEQUENCE
   unsigned int seq_count = 0;

   //This expression looks for and captures
   //the value of an object identifier
   boost::regex expression("OCTET STRING[[:space:]]+:([^\n]+)[\n]?$");

   //This expression looks for the offset, header and size of the
   //Octet String containing the VO
   //The line is of the form
   //<offset>:d=<num>  hl=<header> l=  <length> prim: cont [ 6 ]
   //and occurs one SEQUENCES after the Object Identifier
   //1.3.6.1.4.1.8005.100.100.4
   boost::regex vo_expr("^[[:space:]]*([[:digit:]]+):"
                        ".*hl=[[:space:]]*([[:digit:]]+)[[:space:]]*"
                        "l=[[:space:]]*([[:digit:]]+).*prim:.*");

   std::size_t cur_pos = 0;
   bool fqan_found = false;

   //Grab the parsed output one line at a time
   while ( cur_pos != std::string::npos )
   {
      std::size_t pos = asn1_output.find('\n', cur_pos);
      std::string cur_str;

      if ( pos == std::string::npos )
      {
         cur_str = asn1_output.substr(cur_pos, pos);
         cur_pos = pos;
      }

      else
      {
         //Allow for the 0 ordering of strings
         cur_str = asn1_output.substr(cur_pos, pos - cur_pos + 1);
         cur_pos = pos + 1;
      }

      //Look for the FQAN identifier
      if ( fqan_found == false )
      {
         if ( cur_str.find(fqan) != std::string::npos )
         {
            fqan_found = true;
            continue;
         }
      }

      else
      {
         if ( cur_str.find("SEQUENCE") != std::string::npos )
         {
            ++seq_count;
            continue;
         }

         boost::match_results<std::string::const_iterator> what; 
         boost::match_flag_type flags = boost::match_default; 

         if ( seq_count == 1 )
         {
            if ( regex_search(cur_str,
                              what,
                              vo_expr,
                              flags) == true )
            {
               std::istringstream tmp_iss;

               std::string offset(what[1].first, what[1].second);
               tmp_iss.str(offset);
               unsigned int offset_val;
               tmp_iss >> offset_val;

               std::string header_len(what[2].first, what[2].second);
               tmp_iss.str(header_len);
               tmp_iss.clear();
               unsigned int header_len_val;
               tmp_iss >> header_len_val;

               std::string data_len(what[3].first, what[3].second);
               tmp_iss.str(data_len);
               tmp_iss.clear();
               unsigned int data_len_val;
               tmp_iss >> data_len_val;

               ASN1_STRING* vo_asn1_str = ASN1_STRING_new();
               ASN1_STRING_set(vo_asn1_str,
                               &(asn1_data[offset_val + header_len_val]),
                               data_len_val);

               BIO* bio_mem = BIO_new(BIO_s_mem());

               if ( bio_mem == NULL )
               {
                  error_str = "Unable to create a memory BIO";
                  return false;
               }

               //Parse this string into our memory BIO
               ASN1_STRING_print(bio_mem, vo_asn1_str);

               unsigned int bio_data_len;
               char* bio_data_ptr = NULL;
               bio_data_len = BIO_get_mem_data(bio_mem, &bio_data_ptr);

               //Store the results into our return vector
               vos.push_back(std::string(bio_data_ptr, bio_data_len));

               //Free up all allocated items that we've finished with
               BIO_free(bio_mem);
               ASN1_STRING_free(vo_asn1_str);
               vo_asn1_str = NULL;
               bio_mem = NULL;
               bio_data_ptr = NULL;
            }
         }

         if ( seq_count == 2 )
         {
            if ( regex_search(cur_str,
                              what,
                              expression,
                              flags) == true )
            {
               fqans.push_back(std::string(what[1].first,
                                           what[1].second));
            }
         }

         else if ( seq_count > 2 )
         {
            break;
         }
      }
   }
   return true;
}
} //namespace

