AC_PREREQ(2.61)
AC_INIT([GENIVI Common API C++ Library],
        [2.1.3],
        [],
        [CommonAPI])

AC_CONFIG_MACRO_DIR([m4])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
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





GTEST_MIN_VERSION="1.6.0"
GTEST_URL="http://code.google.com/p/googletest"


AC_ARG_VAR([GTEST_CONFIG], [The exact path of Google Test's 'gtest-config' script.])
AC_ARG_VAR([GTEST_CPPFLAGS], [C-like preprocessor flags for Google Test.])
AC_ARG_VAR([GTEST_CXXFLAGS], [C++ compile flags for Google Test.])
AC_ARG_VAR([GTEST_LDFLAGS], [Linker path and option flags for Google Test.])
AC_ARG_VAR([GTEST_LIBS], [Library linking flags for Google Test.])
AC_ARG_VAR([GTEST_VERSION], [The available version of Google Test.])

AS_IF([test -f "${GTEST_CONFIG}"],
      [AS_IF([${GTEST_CONFIG} --min-version=${GTEST_MIN_VERSION}],
             [],
             [AC_MSG_ERROR([dnl
                            Found Google Test Version ${GTEST_VERSION}. However ${GTEST_MIN_VERSION} is
                            required. Please refer to ${GTEST_URL} for a more recent version.])])
       [GTEST_CPPFLAGS=`${GTEST_CONFIG} --cppflags`]
       [GTEST_CXXFLAGS=`${GTEST_CONFIG} --cxxflags`]
       [GTEST_LDFLAGS=`${GTEST_CONFIG} --ldflags`]
       [GTEST_LIBS=`${GTEST_CONFIG} --libs`]
       [GTEST_VERSION=`${GTEST_CONFIG} --version`]
      ]
      []
)

AM_CONDITIONAL(ENABLE_TESTS, [test -f "${GTEST_CONFIG}"])

# Doxygen support
DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(OFF)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(OFF)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN(${PACKAGE_NAME}, doxygen.cfg, doc)

AC_MSG_RESULT([
        $PACKAGE_NAME v$VERSION
        
        enable docs:       ${ENABLE_DOCS}
		
        GTEST_CONFIG:      ${GTEST_CONFIG}
        GTEST_CPPFLAGS:    ${GTEST_CPPFLAGS}
        GTEST_CXXFLAGS:    ${GTEST_CXXFLAGS}
        GTEST_LDFLAGS:     ${GTEST_LDFLAGS}
        GTEST_LIBS:        ${GTEST_LIBS}
        GTEST_VERSION:     ${GTEST_VERSION}

        prefix:             ${prefix}
        CXXFLAGS:           ${CXXFLAGS}
        LDFLAGS:            ${LDFLAGS}
])







AC_OUTPUT
