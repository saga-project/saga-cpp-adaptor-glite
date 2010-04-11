#  Copyright (c) 2009 Ole Weidner (oweidner@cct.lsu.edu)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

ifndef SAGA_LOCATION
$(warning "")
$(warning " ================================= ")
$(warning "  you need to set SAGA_LOCATION    ")
$(warning " ================================= ")
$(warning "")
$(error --)
endif

include $(SAGA_LOCATION)/share/saga/make/saga.util.mk

SAGA_SUBDIRS = cream_job 

# all:: config.summary
# 
# config.summary:
# 	@$(ECHO) ""
# 	@$(ECHO) " ================================= "
# 	@$(ECHO) "  you need to run configure first  "
# 	@$(ECHO) " ================================= "
# 	@$(ECHO) ""
# 	@$(FALSE)

-include $(SAGA_MAKE_INCLUDE_ROOT)/saga.mk
-include $(SAGA_LOCATION)/share/saga/make/saga.dist.mk

