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
#ifndef INC_SDLEAFNODE_HPP__
#define INC_SDLEAFNODE_HPP__

#include "antlr/CommonAST.hpp"
#include "sd_node.hpp"
#include <string>
#include <list>

using namespace std;

/** A leaf node to represent service comparison */
class sd_leafnode : public sd_node {

public:
        typedef enum
        {
           ANY_VALUE = 0,
           ALL_VALUE
        } ANYALLTYPE;

        sd_leafnode()
        {
           not_prefix = 0;
           //Default to Any value for vector operations
           setAnyAllValue(ANY_VALUE);

           //Default 'LIKE' escape char to '\0'
           setEscapeChar('\0');
        }

        void setRValue(const ANTLR_USE_NAMESPACE(std)string& rval) 
        {
           rvalue = rval;
        }

        void setLValue(int lval) 
        {
           lvalue = lval;
        }

        void setLKey(const ANTLR_USE_NAMESPACE(std)string& lk) 
        {
           lkey = lk;
        }

        void setRValNum(double rval) 
        {
           rvalnum = rval;
        }

        void setRValType(int rvtype) 
        {
           rval_type = rvtype;
        }

        void setOpType(int op) 
        {
           op_type = op;
        }

        void setNotPrefix()
        {
           not_prefix = 1;
        }

        int getNotPrefix()
        {
           return not_prefix;
        }

        ANTLR_USE_NAMESPACE(std)string getRValue()
        {
            return rvalue; 
        }

        ANTLR_USE_NAMESPACE(std)string getLKey()
        {
            return lkey; 
        }

        double getRValNum()
        {
            return rvalnum; 
        }

        int getLValue()
        {
            return lvalue; 
        }

        int getOpType()
        {
            return op_type; 
        }

        int getRValType()
        {
            return rval_type; 
        }

        ANTLR_USE_NAMESPACE(std)list<ANTLR_USE_NAMESPACE(std)string>
           ::const_iterator getListBegin()
        {
            return in_list.begin();
        }

        ANTLR_USE_NAMESPACE(std)list<ANTLR_USE_NAMESPACE(std)string>
           ::const_iterator getListEnd()
        {
            return in_list.end();
        }

        ANTLR_USE_NAMESPACE(std)size_t getListSize()
        {
            return in_list.size();
        }

        void addListElement(const ANTLR_USE_NAMESPACE(std)string& val) 
        {
            in_list.push_back(val);
        }

        ANTLR_USE_NAMESPACE(std)string frontListElement()
        {
            ANTLR_USE_NAMESPACE(std)string s = "";
        
            if (in_list.size() > 0 )
            {       s = in_list.front();
            }
            return s;
        }

        static ANTLR_USE_NAMESPACE(antlr)RefAST factory( void )
        {
                ANTLR_USE_NAMESPACE(antlr)RefAST ret(new sd_leafnode());
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

        ANYALLTYPE getAnyValue()
        {
           return AAValue;
        }

        void setAnyAllValue(ANYALLTYPE AAVal)
        {
           AAValue = AAVal;
        }

        char getEscapeChar()
        {
           return escapeChar;
        }

        void setEscapeChar(char escChar)
        {
           escapeChar = escChar;
        }

private:
        string rvalue;       // Literal
        int lvalue;   	     // Service keyword token type
        int op_type;         // Operator token type 
        int rval_type;       // GlueDataValue Type LITERAL or NUMBER?
        double rvalnum;      // GlueDataValue (Number) 
        string lkey;         // GlueDataKey
        list<string> in_list;// List of literals for IN operator   
        int not_prefix;      // NOT prefix for IN and LIKE operator
        ANYALLTYPE AAValue;  // Is this working on any vector value
        char escapeChar;
};

typedef ANTLR_USE_NAMESPACE(antlr)ASTRefCount<sd_leafnode> Refsd_leafnode;

#endif //INC_SD_LEAFNODE
