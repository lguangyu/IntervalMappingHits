#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([interval_mapping_hits], [0.1], [https://github.com/lguangyu/IntervalMappingHits/issues])
AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SRCDIR([src], [tests])
AC_CONFIG_HEADERS([config.h])
#LT_INIT

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AR

# Checks for libraries.
# Checks for header files.
AX_WITH_HTSLIB
if test "$ax_cv_htslib" != "yes"; then
	AC_MSG_ERROR(["no htslib found"])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.

AC_OUTPUT(Makefile)
