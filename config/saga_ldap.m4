
#
# SYNOPSIS
#
#   AX_SAGA_CHECK_LDAP([MINIMUM-VERSION])
#
# DESCRIPTION
#
#   Test for the LDAP libraries of a particular version (or newer)
#
#   If no path to the installed ldap library is given,
#   the macro searchs under /usr, /usr/local, /opt, /usr/ldap,
#   /usr/local/ldap, /opt/ldap, and /usr/local/package/ldap-*
#   LDAP_LOCATION are evaluated, in that order.
#
#   This macro calls:
#
#     AC_SUBST(SAGA_HAVE_LDAP)
#     AC_SUBST(LDAP_LOCATION)
#     AC_SUBST(LDAP_VERSION) 
#     AC_SUBST(LDAP_CPPFLAGS) 
#     AC_SUBST(LDAP_LDFLAGS) 
#
# LAST MODIFICATION
#
#   2008-09-26
#
# COPYLEFT
#
#   Copyright (c) 2007 Andre Merzky      <andre@merzky.net>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([AX_SAGA_CHECK_LDAP],
[
  AC_ARG_VAR([LDAP_LOCATION],[ldap installation directory])

  SAGA_HAVE_LDAP=no

  tmp_location=""
  AC_ARG_WITH([ldap],
              AS_HELP_STRING([--with-ldap=DIR],
              [use ldap (default is YES) at DIR (optional)]),
              [
              if test "$withval" = "no"; then
                want_ldap="no"
              elif test "$withval" = "yes"; then
                want_ldap="yes"
                tmp_location=""
              else
                want_ldap="yes"
                tmp_location="$withval"
              fi
              ],
              [want_ldap="yes"])

  # use LDAP_LOCATION if avaialble, and if not 
  # overwritten by --with-ldap=<dir>

  if test "x$want_ldap" = "xyes"; then
    
    packages=`ls /usr/local/package/ldap-* 2>>/dev/null`

    if test "$tmp_location-$LDAP_LOCATION-$ldap" = "--"; then
      paths="/usr /usr/local /opt /sw $packages /usr/ldap /usr/local/ldap /opt/ldap"
    else
      paths="$tmp_location $LDAP_LOCATION $ldap"
    fi

    for tmp_path in $paths; do
      
      AC_MSG_CHECKING(for ldap.h in $tmp_path/include)
    
      if test -e "$tmp_path/include/ldap.h"; then

        AC_MSG_RESULT(yes)
        SAGA_HAVE_LDAP=yes
        LDAP_LOCATION=$tmp_path

        saved_ldflags=$LDFLAGS  
        saved_cppflags=$CPPFLAGS 

        LDAP_LDFLAGS="-L$tmp_path/lib -lldap"
        LDAP_CPPFLAGS="-I$tmp_path/include/"

        CPPFLAGS="$CPPFLAGS $LDAP_CPPFLAGS"
        export CPPFLAGS

        LDFLAGS="$LDFLAGS $LDAP_LDFLAGS"
        export LDFLAGS

        AC_MSG_CHECKING(for libldap in $tmp_path/lib)

        AC_LINK_IFELSE([AC_LANG_PROGRAM([[@%:@include <ldap.h>]],
                                        [[return ldap_create(NULL);
                                        ]])],
                                        link_ldap="yes",
                                        link_ldap="no")
              
        if test "x$link_ldap" = "xno"; then
        
          AC_MSG_RESULT(no)

          LDFLAGS=$saved_ldflags
          CPPFLAGS=$saved_cppflags
        
        else


          LDAP_VERSION_MAJ=`grep LDAP_VENDOR_VERSION_MAJOR $tmp_path/include/ldap_features.h | rev | cut -f 1 -d ' ' | rev`
          LDAP_VERSION_MIN=`grep LDAP_VENDOR_VERSION_MINOR $tmp_path/include/ldap_features.h | rev | cut -f 1 -d ' ' | rev`
          LDAP_VERSION_SUB=`grep LDAP_VENDOR_VERSION_PATCH $tmp_path/include/ldap_features.h | rev | cut -f 1 -d ' ' | rev`

          LDAP_VERSION="$LDAP_VERSION_MAJ.$LDAP_VERSION_MIN.$LDAP_VERSION_SUB"

          AC_MSG_RESULT([yes ($LDAP_VERSION)])

          SAGA_LDAP_DEP_FILES="$tmp_path/include/ldap.h"
          break;
        fi
      fi


      AC_MSG_RESULT(no)

    done # foreach path

  fi # want_ldap


  if  test "$SAGA_HAVE_LDAP" != "yes"; then

    AC_MSG_WARN([no ldap found])

  fi


  AC_SUBST(SAGA_HAVE_LDAP)
  AC_SUBST(LDAP_LOCATION)
  AC_SUBST(LDAP_CPPFLAGS)
  AC_SUBST(LDAP_LDFLAGS)
  AC_SUBST(LDAP_VERSION)

])

