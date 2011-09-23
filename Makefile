#  Copyright (c) 2005-2006 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

-include $(SAGA_LOCATION)/share/saga/make/saga.dist.mk

include config/make.cfg

SAGA_SUBDIRS = config external

ifeq "x$(BUILD_ADAPTOR_JOB)" "xyes"
  $(warning $(BUILD_ADAPTOR_JOB))
  SAGA_SUBDIRS += cream_job
endif

ifeq "x$(BUILD_ADAPTOR_SD)" "xyes"
  SAGA_SUBDIRS += glite-sd
endif

ifeq "x$(BUILD_ADAPTOR_ISN)" "xyes"
  SAGA_SUBDIRS += glite-isn
endif

all:: config/make.cfg

ifndef SAGA_IS_PACKAGING
config/make.cfg: 
	@echo ""
	@echo " ================================= "
	@echo "  you need to run configure first  "
	@echo " ================================= "
	@echo ""
	@false
endif


-include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk

