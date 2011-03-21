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
#ifndef INC_ISN_INTERNALNODE_HPP__

#define INC_ISN_INTERNALNODE_HPP__

#include "antlr/CommonAST.hpp"
#include "isn_node.hpp"
#include <string>

using namespace std;

/** An internal node to represent OR / AND operator */
class isn_internalnode : public isn_node {

public:

     isn_internalnode()
     {
     }

     static ANTLR_USE_NAMESPACE(antlr)RefAST factory( void )
     {
	     ANTLR_USE_NAMESPACE(antlr)RefAST ret(new isn_internalnode());
	     return ret;
     }

     void initialize(int t, const ANTLR_USE_NAMESPACE(std)string& txt)
     {
	    CommonAST::initialize(t, txt);
     }

     void initialize(ANTLR_USE_NAMESPACE(antlr)RefAST t)
     {
	     CommonAST::initialize(t);
     }
     void initialize(ANTLR_USE_NAMESPACE(antlr)RefToken tok)
     {
	     CommonAST::initialize(tok);
     }

};

typedef ANTLR_USE_NAMESPACE(antlr)ASTRefCount<isn_internalnode> Refisn_internalnode;

#endif
