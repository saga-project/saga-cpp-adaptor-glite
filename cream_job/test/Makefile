#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

ifndef SAGA_LOCATION
 $(error "SAGA_LOCATION must be set for this Makefile to work!")
endif


SAGA_SRC         = $(wildcard *.cpp)
SAGA_OBJ         = $(SAGA_SRC:%.cpp=%.o)
SAGA_BIN         = $(SAGA_SRC:%.cpp=%)

SAGA_TEST_RUN           = ./run_adaptor_tests.sh.run
SAGA_DONT_INSTALL       = yes

include $(SAGA_LOCATION)/share/saga/make/saga.test.mk
