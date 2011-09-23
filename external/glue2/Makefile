#  Copyright (c) 2005-2008 Andre Merzky (andre@merzky.net)
# 
#  Use, modification and distribution is subject to the 
#  Boost Software License, Version 1.0. (See accompanying 
#  file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)

include ../../config/make.cfg

SAGA_XML        = $(wildcard *.xml)
SAGA_XML_DIR    = $(SAGA_DAT_ROOT)/schema/glue1/

include $(SAGA_LOCATION)/share/saga/make/saga.mk

install::
	@$(ECHO) "      installing   GLUE-v.2 schema"
	@$(INSTALL)     $(SAGA_XML)  $(SAGA_XML_DIR)

