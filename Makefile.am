ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = LICENSE
MOSTLYCLEANFILES =
# Library interface (not package version!)
LIBCOMMONAPI_CURRENT=3
LIBCOMMONAPI_REVISION=1
LIBCOMMONAPI_AGE=0

AM_CPPFLAGS = \
        -DCOMMONAPI_INTERNAL_COMPILATION

# ------------------------------------------------------------------------------
# Doxygen
include $(top_srcdir)/aminclude.am 
EXTRA_DIST += doxygen.cfg
MOSTLYCLEANFILES += ${DX_CLEANFILES}

# ------------------------------------------------------------------------------
MAINTAINERCLEANFILES = \
        Makefile.in \
        aclocal.m4 \
        configure

clean-local:
	-rm -rf src-gen

maintainer-clean-local:
	-rm -rf build-aux
	-rm -f config.h.in*
	-rm -f m4/libtool*.m4
	-rm -f m4/lt*.m4


lib_LTLIBRARIES = libCommonAPI.la

libCommonAPI_la_SOURCES = \
        src/CommonAPI/Runtime.cpp \
        src/CommonAPI/Configuration.cpp \
        src/CommonAPI/ServicePublisher.cpp \
        src/CommonAPI/ContainerUtils.cpp \
        src/CommonAPI/MainLoopContext.cpp \
        src/CommonAPI/InputStream.cpp \
        src/CommonAPI/Factory.cpp \
        src/CommonAPI/utils.cpp

CommonAPI_includedir=$(includedir)/CommonAPI-${COMMONAPI_API_HEADER_VERSION}/CommonAPI
CommonAPI_include_HEADERS = \
        src/CommonAPI/Attribute.h \
        src/CommonAPI/AttributeExtension.h \
        src/CommonAPI/ByteBuffer.h \
        src/CommonAPI/CommonAPI.h \
        src/CommonAPI/Configuration.h \
        src/CommonAPI/ContainerUtils.h \
        src/CommonAPI/Event.h \
        src/CommonAPI/Factory.h \
        src/CommonAPI/Factory.hpp \
        src/CommonAPI/InputStream.h \
        src/CommonAPI/MainLoopContext.h \
        src/CommonAPI/MiddlewareInfo.h \
        src/CommonAPI/OutputStream.h \
        src/CommonAPI/Proxy.h \
        src/CommonAPI/ProxyManager.h \
        src/CommonAPI/Runtime.h \
        src/CommonAPI/SelectiveEvent.h \
        src/CommonAPI/SerializableStruct.h \
        src/CommonAPI/SerializableVariant.h \
        src/CommonAPI/SerializableVariant.hpp \
        src/CommonAPI/ServicePublisher.h \
        src/CommonAPI/ServicePublisher.hpp \
        src/CommonAPI/Stub.h \
        src/CommonAPI/types.h \
        src/CommonAPI/utils.h

libCommonAPI_la_LIBADD = -ldl
libCommonAPI_la_LDFLAGS = \
        ${AM_LDFLAGS} \
        -version-info ${LIBCOMMONAPI_CURRENT}:${LIBCOMMONAPI_REVISION}:${LIBCOMMONAPI_AGE}

pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = CommonAPI.pc

# ------------------------------------------------------------------------------
if ENABLE_TESTS

check_PROGRAMS = \
        VariantTest


TESTS = ${check_PROGRAMS}
VariantTest_includedir=$(includedir)/CommonAPI-${COMMONAPI_API_HEADER_VERSION}/CommonAPI
VariantTest_SOURCES = src/test/VariantTest.cpp
VariantTest_CPPFLAGS = ${AM_CPPFLAGS} ${GTEST_CPPFLAGS}
VariantTest_CXXFLAGS = ${GTEST_CXXFLAGS} -Isrc
VariantTest_LDADD = ${LDADD} ${GTEST_LIBS} libCommonAPI.la

endif

doc: doxygen-doc
	asciidoc -a version=@PACKAGE_VERSION@ -b html -o doc/html/README.html README
