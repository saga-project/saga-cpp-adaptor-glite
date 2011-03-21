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
#ifndef ADAPTORS_GLITE_DISCOVERER_HPP

#define ADAPTORS_GLITE_DISCOVERER_HPP

#include <saga/adaptors/adaptor.hpp>
#include <saga/adaptors/adaptor_data.hpp>
#include <saga/session.hpp>

#include <impl/engine/proxy.hpp>
#include <impl/packages/sd/discoverer_cpi.hpp>

#include "glite_sd_adaptor.hpp"

#define GLITE_SD_PROVIDER "GLITE_SD_PROVIDER"

///////////////////////////////////////////////////////////////////////////////////
namespace glite_sd_adaptor  {
class discoverer_cpi_impl
    : public saga::adaptors::v1_0::discoverer_cpi<discoverer_cpi_impl>
{
  private:
     typedef saga::adaptors::v1_0::discoverer_cpi<discoverer_cpi_impl>
         base_cpi;

     // adaptor data
     typedef saga::adaptors::adaptor_data<sd_adaptor> adaptor_data_type;
     //int port;

     boost::shared_ptr<saga::session> _session;

     std::string _proxy_location;

     std::string
        get_glue1_authz_filter(const std::vector<std::string>& vo) const;

     std::string
        get_glue2_authz_filter(const std::vector<std::string>& vo,
                               const std::vector<std::string>& fqan,
                               const std::string& identity) const;

     saga::ini::ini _ini;
     bool _doGlue1;
     bool _doGlue2;

  protected:
     void set_session(const boost::shared_ptr<saga::session>);

  public:
     /*! constructor of the discoverer cpi */
     discoverer_cpi_impl (proxy        * p,
                          cpi_info     const & info,
                          saga::ini::ini const & glob_ini,
                          saga::ini::ini const & adap_ini,
                          TR1::shared_ptr<saga::adaptor> adaptor);

     /*! destructor of the discoverer cpi */
     ~discoverer_cpi_impl (void);

     // CPI functions
     void
        sync_list_services2(std::vector<saga::sd::service_description>& ret,
                            std::string service_filter,
                            std::string data_filter);

     void
        sync_list_services3(std::vector<saga::sd::service_description>& ret,
                            std::string service_filter,
                            std::string vo_filter,
                            std::string data_filter);

}; // class discoverer_cpi_impl
}
#endif
