ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = LICENSE

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
		src/CommonAPI/Runtime.cpp

CommonAPI_includedir=$(includedir)/CommonAPI-${VERSION}/CommonAPI
CommonAPI_include_HEADERS = \
		src/CommonAPI/Attribute.h \
        src/CommonAPI/AttributeExtension.h \
        src/CommonAPI/ByteBuffer.h \
        src/CommonAPI/Event.h \
        src/CommonAPI/Factory.h \
		src/CommonAPI/InputStream.h \
        src/CommonAPI/MiddlewareInfo.h \
		src/CommonAPI/OutputStream.h \
        src/CommonAPI/Proxy.h \
		src/CommonAPI/Runtime.h \
		src/CommonAPI/SerializableStruct.h \
		src/CommonAPI/SerializableVariant.h \
		src/CommonAPI/SerializableVariant.hpp \
		src/CommonAPI/Stub.h \
		src/CommonAPI/types.h

libCommonAPI_la_LIBADD = -ldl

pkgconfigdir = ${libdir}/pkgconfig
pkgconfig_DATA = CommonAPI.pc

# ------------------------------------------------------------------------------
if ENABLE_TESTS

check_PROGRAMS = \
		VariantTest


TESTS =	${check_PROGRAMS}
VariantTest_includedir=$(includedir)/CommonAPI-${VERSION}/CommonAPI
VariantTest_SOURCES = src/test/VariantTest.cpp
VariantTest_CPPFLAGS = ${AM_CPPFLAGS} ${GTEST_CPPFLAGS}
VariantTest_CXXFLAGS = ${GTEST_CXXFLAGS} -Isrc
VariantTest_LDADD = ${LDADD} ${GTEST_LIBS} libCommonAPI.la

endif
