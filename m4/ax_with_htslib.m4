# configure option of --with-htslib
#
# SYNOPSIS
#
#	AX_WITH_HTSLIB
#
# DESCRIPTION
#
# Check if a valid htslib installation can be found; minimal sam.h and htslib.so
# required. Will add '-I/hts/prefix/include' and '-L/hts/prefix/lib' to CPPFLAGS
# and LDFLAGS if succeded.
# Created variables:
#	ax_with_htslib				argument string in --with-htslib
#	ax_cv_htslib				'yes' if found, otherwise 'no'
# Optional environmental variables
#	HTSLIB_CFLAGS
#	HTSLIB_LDFLAGS
# Above environmental variables may be overriden if --with-htslib is also used
AC_DEFUN([AX_WITH_HTSLIB], [
	AC_ARG_VAR([HTSLIB_CFLAGS], [preprocessor flags for htslib, e.g. -I/usr/include])
	AC_ARG_VAR([HTSLIB_LDFLAGS], [linker flags for htslib, e.g. -L/usr/lib])
	AC_ARG_WITH([htslib], [AS_HELP_STRING([--with-htslib=DIR],
		[install prefix of htslib in custom path])])

	if test "x$with_htslib" != "x"; then
		HTSLIB_CFLAGS="-I$with_htslib/include"
		HTSLIB_LDFLAGS="-L$with_htslib/lib"
	fi

	dnl test if working
	ax_saved_CPPFLAGS=$CPPFLAGS
	ax_saved_LDFLAGS=$LDFLAGS
	CPPFLAGS="$CPPFLAGS $HTSLIB_CFLAGS"
	LDFLAGS="$LDFLAGS $HTSLIB_LDFLAGS"
	AC_CHECK_HEADER([htslib/sam.h],
		[AC_CHECK_LIB(hts, hts_version, [ax_cv_htslib=yes], [ax_cv_htslib=no])],
		[ax_cv_htslib=no], [;])

	if test "x$ax_cv_htslib" = "xno"; then
		CPPFLAGS=ax_saved_CPPFLAGS
		LDFLAGS=ax_saved_LDFLAGS
	fi
])dnl
