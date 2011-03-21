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
#ifndef INC_BDII_QUERY_HPP__
#define INC_BDII_QUERY_HPP__

#include "isnTokenTypes.hpp"
#include "isn_node.hpp"
#include "isn_leafnode.hpp"

#include "saga/saga/packages/isn/entity_types.hpp"

ANTLR_USING_NAMESPACE(antlr)

namespace saga { namespace isn { class entity_data; } }

class bdii_query 
{
private:
     string _model;
     string _filter;
     string _type;
     RefAST _top;

     void eval_data_filter(RefAST top,
                           saga::isn::entity_data& data,
                           const std::map<std::string,
                                          ENTITY_ATTR_MAP_TYPE>& attrs);

     //Series of member functions for processing
     //individual parts of a data filter
     bool process_data_in(const std::vector<std::string>& lhs,
                          Refisn_leafnode nodeRef);
     bool process_data_like(const std::vector<std::string>& lhs,
                            Refisn_leafnode nodeRef);
     bool process_data_null(const std::vector<std::string>& lhs,
                            Refisn_leafnode nodeRef);

     //Intentional copy by value
     bool process_data_eq(std::vector<std::string> lhs,
                          Refisn_leafnode nodeRef);
     bool process_data_ne(std::vector<std::string> lhs,
                          Refisn_leafnode nodeRef);

     bool process_data_lt(const std::vector<std::string>& lhs,
                          Refisn_leafnode nodeRef);
     bool process_data_gt(const std::vector<std::string>& lhs,
                          Refisn_leafnode nodeRef);
     bool process_data_le(const std::vector<std::string>& lhs,
                          Refisn_leafnode nodeRef);
     bool process_data_ge(const std::vector<std::string>& lhs,
                          Refisn_leafnode nodeRef);

public:
     bdii_query(const std::string& model,
                const std::string& filter,
                const std::string& type);

     bool evaluate_data_filter(saga::isn::entity_data& data);
};

#endif
