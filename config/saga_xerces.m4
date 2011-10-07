
#
# SYNOPSIS
#
#   AX_SAGA_CHECK_XERCESC([MINIMUM-VERSION])
#
# DESCRIPTION
#
#   Test for the XERCESC libraries of a particular version (or newer)
#
#   If no path to the installed xerces-c library is given,
#   the macro searchs under /usr, /usr/local, /opt, /usr/xerces-c,
#   /usr/local/xerces-c, /opt/xerces-c, and /usr/local/package/xerces-c-*
#   XERCESC_LOCATION are evaluated, in that order.
#
#   This macro calls:
#
#     AC_SUBST(SAGA_HAVE_XERCESC)
#     AC_SUBST(XERCESC_LOCATION)
#     AC_SUBST(XERCESC_VERSION) 
#     AC_SUBST(XERCESC_CPPFLAGS) 
#     AC_SUBST(XERCESC_LDFLAGS) 
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

AC_DEFUN([AX_SAGA_CHECK_XERCESC],
[
  AC_ARG_VAR([XERCESC_LOCATION],[xerces-c installation directory])

  SAGA_HAVE_XERCESC=no

  tmp_location=""
  AC_ARG_WITH([xercesc],
              AS_HELP_STRING([--with-xercesc=DIR],
              [use xercesc (default is YES) at DIR (optional)]),
              [
              if test "$withval" = "no"; then
                want_xerces_c="no"
              elif test "$withval" = "yes"; then
                want_xerces_c="yes"
                tmp_location=""
              else
                want_xerces_c="yes"
                tmp_location="$withval"
              fi
              ],
              [want_xerces_c="yes"])

  # use XERCESC_LOCATION if avaialble, and if not 
  # overwritten by --with-xercesc=<dir>

  if test "x$want_xerces_c" = "xyes"; then
    
    packages=`ls /usr/local/package/xerces-* 2>>/dev/null`

    if test "$tmp_location-$XERCESC_LOCATION-$xercesc" = "--"; then
      paths="/usr /usr/local /opt /sw $packages /usr/xercesc /usr/local/xercesc /opt/xercesc"
    else
      paths="$tmp_location $XERCESC_LOCATION $xercesc"
    fi

    for tmp_path in $paths; do
      
      AC_MSG_CHECKING(for xercesc/util/PlatformUtils.hpp in $tmp_path/include)
    
      if test -e "$tmp_path/include/xercesc/util/PlatformUtils.hpp"; then

        AC_MSG_RESULT(yes)
        SAGA_HAVE_XERCESC=yes
        XERCESC_LOCATION=$tmp_path

        saved_ldflags=$LDFLAGS  
        saved_cppflags=$CPPFLAGS 

        XERCESC_LDFLAGS="-L$tmp_path/lib -lxerces-c"
        XERCESC_CPPFLAGS="-I$tmp_path/include/"

        CPPFLAGS="$CPPFLAGS $XERCESC_CPPFLAGS"
        export CPPFLAGS

        LDFLAGS="$LDFLAGS $XERCESC_LDFLAGS"
        export LDFLAGS

        AC_MSG_CHECKING(for libxerces-c in $tmp_path/lib)

        AC_LINK_IFELSE([AC_LANG_PROGRAM([[@%:@include <xercesc/util/PlatformUtils.hpp>
                                          #ifdef XERCES_CPP_NAMESPACE_USE
                                           XERCES_CPP_NAMESPACE_USE
                                          #endif]],
                                        [[XMLPlatformUtils::Initialize();
                                          return 0;
                                        ]])],
                                        link_xerces_c="yes",
                                        link_xerces_c="no")
              
        if test "x$link_xerces_c" = "xno"; then
        
          AC_MSG_RESULT(no)

          LDFLAGS=$saved_ldflags
          CPPFLAGS=$saved_cppflags
        
        else


          XERCESC_VERSION_MAJ=`grep " XERCES_VERSION_MAJOR "    $tmp_path/include/xercesc/util/XercesVersion.hpp | grep -e '^#define' | rev | cut -f 1 -d ' ' | rev`
          XERCESC_VERSION_MIN=`grep " XERCES_VERSION_MINOR "    $tmp_path/include/xercesc/util/XercesVersion.hpp | grep -e '^#define' | rev | cut -f 1 -d ' ' | rev`
          XERCESC_VERSION_SUB=`grep " XERCES_VERSION_REVISION " $tmp_path/include/xercesc/util/XercesVersion.hpp | grep -e '^#define' | rev | cut -f 1 -d ' ' | rev`

          XERCESC_VERSION="$XERCESC_VERSION_MAJ.$XERCESC_VERSION_MIN.$XERCESC_VERSION_SUB"

          AC_MSG_RESULT([yes ($XERCESC_VERSION)])

          SAGA_XERCESC_DEP_FILES="$tmp_path/include/xercesc/util/PlatformUtils.hpp"
          break;
        fi
      fi


      AC_MSG_RESULT(no)

    done # foreach path

  fi # want_xerces_c


  if  test "$SAGA_HAVE_XERCESC" != "yes"; then

    AC_MSG_WARN([no xerces-c found])

  fi


  AC_SUBST(SAGA_HAVE_XERCESC)
  AC_SUBST(XERCESC_LOCATION)
  AC_SUBST(XERCESC_CPPFLAGS)
  AC_SUBST(XERCESC_LDFLAGS)
  AC_SUBST(XERCESC_VERSION)

])

