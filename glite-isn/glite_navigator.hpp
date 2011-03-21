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
#ifndef _GLITE_NAVIGATOR_HPP_
#define _GLITE_NAVIGATOR_HPP_

#include <saga/adaptors/adaptor.hpp>
#include <saga/adaptors/adaptor_data.hpp>
#include <saga/session.hpp>

#include <impl/engine/proxy.hpp>
#include <impl/packages/isn/navigator_cpi.hpp>
#include <saga/saga/packages/isn/entity_data.hpp>

#include "glite_isn_adaptor.hpp"

#include "ldap_provider.hpp"
#include "config_handler.hpp"
#include "bdii_query.hpp"

#define GLITE_ISN_PROVIDER "GLITE_ISN_PROVIDER"


///////////////////////////////////////////////////////////////////////////////////
namespace glite_isn_adaptor  {
class navigator_cpi_impl
   : public saga::adaptors::v1_0::navigator_cpi<navigator_cpi_impl>
{
  private:
     typedef saga::adaptors::v1_0::navigator_cpi<navigator_cpi_impl> base_cpi;

    // adaptor data
    typedef saga::adaptors::adaptor_data<isn_adaptor> adaptor_data_type;

    boost::shared_ptr<saga::session> _session;

    std::string _proxy_location;

    ldap_provider* _ldap;
    std::string _configDirectory;
    std::string _model;

  protected:
     void set_session(const boost::shared_ptr<saga::session>);

  public:
     /*! constructor of the navigator cpi */
     navigator_cpi_impl (proxy        * p,
                         cpi_info     const & info,
                         saga::ini::ini const & glob_ini,
                         saga::ini::ini const & adap_ini,
                         TR1::shared_ptr<saga::adaptor> adaptor);

     /*! destructor of the navigator cpi */
     ~navigator_cpi_impl (void);

     // CPI functions
     void sync_get_entities(std::vector<saga::isn::entity_data>& ret,
                            std::string entityType,
                            std::string filter);

     void sync_list_related_entity_names(std::vector<std::string>& ret,
                                         std::string entity);

     void sync_get_related_entities(std::vector<saga::isn::entity_data>& ret,
                                    std::string entityName,
                                    std::string relatedEntityName,
                                    std::string filter,
                                    std::vector<saga::isn::entity_data>
                                       entities);
}; // class navigator_cpi_impl
}

#endif

