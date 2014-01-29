cmake_minimum_required (VERSION 2.8.1)
FIND_PACKAGE(PkgConfig)
set(CAPI_LIB_SRCS Runtime.cpp Configuration.cpp ServicePublisher.cpp ContainerUtils.cpp )
add_library(CommonAPI ${CAPI_LIB_SRCS})
set_target_properties(CommonAPI PROPERTIES VERSION 3.0.0 SOVERSION 3 LINKER_LANGUAGE C)

set(CAPI_DEV_INCL *.h )

install(TARGETS CommonAPI
	LIBRARY DESTINATION /usr/lib
	ARCHIVE DESTINATION lib/static
	COMPONENT base)	
