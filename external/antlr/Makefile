#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
#
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

-include ../../config/make.cfg

SAGA_ARC          = saga_antlr
SAGA_SRC          = $(wildcard *.cpp)
SAGA_SRC         := $(filter-out dll.cpp, $(SAGA_SRC))
SAGA_LIB_OBJ     += $(SAGA_SRC:%.cpp=%.o)

SAGA_DONT_INSTALL = 1

-include $(SAGA_LOCATION)/share/saga/make/saga.mk

SAGA_CPPINCS += -I../

