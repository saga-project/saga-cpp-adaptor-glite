#
# Copyright (c) Members of the EGEE Collaboration. 2009-2010.
# See http://www.eu-egee.org/partners/ for details on the copyright
# holders.
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# isn-cpp Makefile
#
# Note that changes to headers will not trigger re-compilation.

-include ../config/make.cfg

GRAMMAR_FILE              = isn_grammar.g
GRAMMAR_SRC               = isn_parser.cpp isn_lexer.cpp 
GRAMMAR_HDR               = isn_parser.hpp isn_lexer.hpp isnTokenTypes.hpp 
GRAMMAR_TXT               = isnTokenTypes.txt

SAGA_HDR                  = $(sort $(wildcard *.hpp) $(GRAMMAR_HDR))
SAGA_SRC                  = $(sort $(wildcard *.cpp) $(GRAMMAR_SRC))
SAGA_OBJ                  = $(SAGA_SRC:%.cpp=%.o)

SAGA_ADAPTOR_OBJ          = $(SAGA_OBJ)


SAGA_ADAPTOR_TYPE         = isn 
SAGA_ADAPTOR_NAME         = glite_isn
SAGA_ADAPTOR_INI          = glite_isn_adaptor.ini

SAGA_ADAPTOR_PACKAGES     = isn


-include $(SAGA_LOCATION)/share/saga/make/saga.adaptor.mk

SAGA_ADAPTOR_REQUIREMENTS = openssl

SAGA_CXXFLAGS += $(SAGA_LDAP_CPPFLAGS) -DLDAP_DEPRECATED
SAGA_LDFLAGS  += $(SAGA_LDAP_LDFLAGS)

SAGA_CXXFLAGS += $(SAGA_XERCESC_CPPFLAGS)
SAGA_LDFLAGS  += $(SAGA_XERCESC_LDFLAGS)

SAGA_CXXFLAGS += -I../external/
SAGA_LDFLAGS  += -L../external/antlr/ -lsaga_antlr


# needed?
# ifeq "$(SAGA_OSTYPE)" "Darwin"
#   SAGA_LDFLAGS   += -undefined dynamic_lookup
# endif
#
# -DSIZE_T_DEFINED 


.PHONY: grammar
grammar::
	@$(ECHO) "      re-building  grammar" 
	java -cp ../external/antlr/antlr.jar antlr.Tool $(GRAMMAR_FILE)

.PHONY: grammar-clean
grammar-clean::
	@$(RM)   $(GRAMMAR_SRC) $(GRAMMAR_HDR) $(GRAMMAR_TXT) $(GRAMMAR_OBJ) 

