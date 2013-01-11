AC_PREREQ(2.61)
AC_INIT([GENIVI Common API C++ Library],
        [0.7],
        [],
        [CommonAPI])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([build-aux/config.h])
AC_CONFIG_FILES([
		Makefile
		CommonAPI.pc
		CommonAPI-uninstalled.pc])

AC_CANONICAL_SYSTEM

AM_INIT_AUTOMAKE([foreign 1.11 silent-rules subdir-objects -Wall -Werror])
AM_SILENT_RULES
AM_MAINTAINER_MODE

AC_PROG_CXX
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11([noext])

LT_PREREQ(2.2)
LT_INIT

AC_MSG_RESULT([
        $PACKAGE_NAME v$VERSION

        prefix:             ${prefix}
        CXXFLAGS:           ${CXXFLAGS}
        LDFLAGS:            ${LDFLAGS}
])

AC_OUTPUT

