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
#ifndef INC_SD_BDII_QUERY_HPP__
#define INC_SD_BDII_QUERY_HPP__

#include <saga/adaptors/task.hpp>
#include <saga/adaptors/attribute.hpp>
#include "sdTokenTypes.hpp"
#include "sd_node.hpp"
#include "sd_leafnode.hpp"
#include "sd_internalnode.hpp"
#include <saga/packages/sd/service_description.hpp>

#include <map>

ANTLR_USING_NAMESPACE(antlr)

class sd_bdii_query
{
private:
     string svc_filter;
     string vo_filter;
     string data_filter;

     std::multimap<std::string, std::string> capability_map;
     typedef std::multimap<std::string, std::string>::const_iterator CAP_CITER;

     std::string get_capability_string(const std::string& capability) const;
     string get_svc_keyword(int ktoken) const;
     std::vector<std::string>
        get_service_values(const saga::sd::service_description& sd,
                           int ktoken);

     void find_and_replace(string& source,
                           const string& fstr,
                           const string& rstr);
     void eval_service_filter(RefAST top, saga::sd::service_description& sd);

     void eval_authz_filter(RefAST top,
                            saga::sd::service_description& sd,
                            bool allFlag,
                            const std::set<std::string>& vo,
                            const std::set<std::string>& voms,
                            const std::set<std::string>& fqan,
                            const std::set<std::string>& dn);

     void eval_data_filter(RefAST top, saga::sd::service_data& sd);
     void generate_svc_filter(RefAST top);
     std::string correct_svc_filter(const std::string& filter);
     void generate_authz_filter(RefAST top);

     void get_authz_attributes(const saga::sd::service_description& sd,
                               bool& allFlag,
                               std::set<std::string>& vo,
                               std::set<std::string>& voms,
                               std::set<std::string>& fqan,
                               std::string& dn) const;

     //Series of member functions for processing
     //individual parts of a data filter
     bool process_data_in(const std::vector<std::string>& lhs,
                          Refsd_leafnode nodeRef,
                          bool passFlag = false) const;
     bool process_data_like(const std::vector<std::string>& lhs,
                            Refsd_leafnode nodeRef,
                            bool passFlag = false) const;
     bool process_data_null(const std::vector<std::string>& lhs,
                            Refsd_leafnode nodeRef,
                            bool passFlag = false) const;

     //Intentional copy by value
     bool process_data_eq(std::vector<std::string> lhs,
                          Refsd_leafnode nodeRef,
                          bool passFlag = false) const;
     bool process_data_ne(std::vector<std::string> lhs,
                          Refsd_leafnode nodeRef,
                          bool passFlag = false) const;

     bool process_data_lt(const std::vector<std::string>& lhs,
                          Refsd_leafnode nodeRef) const;
     bool process_data_gt(const std::vector<std::string>& lhs,
                          Refsd_leafnode nodeRef) const;
     bool process_data_le(const std::vector<std::string>& lhs,
                          Refsd_leafnode nodeRef) const;
     bool process_data_ge(const std::vector<std::string>& lhs,
                          Refsd_leafnode nodeRef) const;

     //GLUE version to deal with
     unsigned int _glueVersion;

public:
     sd_bdii_query(unsigned int glueVersion);
     void initialize();
     string get_svc_filter(RefAST top);
     string get_authz_filter(RefAST top);
     bool evaluate_service_filter(RefAST top,
                                  saga::sd::service_description& sd);
     bool evaluate_authz_filter(RefAST top,
                                saga::sd::service_description& sd,
                                bool allFlag,
                                const std::set<std::string>& vo,
                                const std::set<std::string>& voms,
                                const std::set<std::string>& fqan,
                                const std::set<std::string>& dn);

     bool evaluate_data_filter(RefAST top, saga::sd::service_data& sd);
};

#endif
