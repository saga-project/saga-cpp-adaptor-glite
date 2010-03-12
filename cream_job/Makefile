#  Copyright (c) 2009 Ole Weidner (oweidner@cct.lsu.edu)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

ifndef SAGA_LOCATION
 $(error "SAGA_LOCATION must be set for this Makefile to work!")
endif

ifndef CREAM_CLIENT_API_C_LOCATION
 $(error "CREAM_CLIENT_API_C_LOCATION must be set for this Makefile to work!")
endif

ifndef GSOAP_LOCATION
 $(error "GSOAP_LOCATION must be set for this Makefile to work!")
endif

ifndef LOG4CPP_LOCATION
 $(error "LOG4CPP_LOCATION must be set for this Makefile to work!")
endif

SAGA_SRC          = $(wildcard *.cpp)
SAGA_OBJ          = $(SAGA_SRC:%.cpp=%.o)

SAGA_ADAPTOR_TYPE = job
SAGA_ADAPTOR_NAME = glite_cream_job
SAGA_ADAPTOR_INI  = glite_cream_job_adaptor.ini
SAGA_ADAPTOR_OBJ  = $(SAGA_OBJ)

SAGA_ADAPTOR_PACKAGES = job

include $(SAGA_LOCATION)/share/saga/make/saga.adaptor.mk

SAGA_CXXFLAGS += -I$(CREAM_CLIENT_API_C_LOCATION)/include/
SAGA_CXXFLAGS += -I$(GSOAP_LOCATION)/include/
SAGA_CXXFLAGS += -I$(LOG4CPP_LOCATION)/include/

SAGA_LDFLAGS  += -L$(CREAM_CLIENT_API_C_LOCATION)/lib64/ -lglite_ce_cream_client_soap -lglite_ce_cream_client_util -lvomsc_gcc64dbgpthr
SAGA_LDFLAGS  += -L$(CREAM_CLIENT_API_C_LOCATION)/lib/ -lglite_security_gsoap_plugin_276b_gcc64dbgpthr
SAGA_LDFLAGS  += -L$(LOG4CPP_LOCATION)/lib/ -llog4cpp

