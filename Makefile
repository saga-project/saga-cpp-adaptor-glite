#  Copyright (c) 2009 Ole Weidner (oweidner@cct.lsu.edu)
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

ifndef SAGA_LOCATION
 $(error "SAGA_LOCATION must be set for this Makefile to work!")
endif

SAGA_SUBDIRS   = glite_cream_job 

#install::
#	echo XX

include $(SAGA_LOCATION)/share/saga/make/saga.mk

