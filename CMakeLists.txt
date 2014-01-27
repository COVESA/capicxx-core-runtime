cmake_minimum_required (VERSION 2.8.1)

PROJECT(libcommonapi)

SET( ${PROJECT_NAME}_MAJOR_VERSION 2 )
SET( ${PROJECT_NAME}_MINOR_VERSION 1 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 4 ) 

set(GENIVI_PROJECT_VERSION ${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}.${${PROJECT_NAME}_PATCH_LEVEL})

set(GENIVI_RPM_RELEASE ${GENIVI_PROJECT_VERSION})

##################### RPM CONFIG ########################
#SET( GENIVI_RPM_RELEASE "1${SVN_REVISION}" )
SET( LICENSE "MPL2" )
#########################################################


SET( prefix ${CMAKE_INSTALL_PREFIX})
SET( libdir "\${exec_prefix}/lib" )
SET( includedir "\${exec_prefix}/include" )

SET( SHMLIBDIR ${CMAKE_INSTALL_PREFIX}/lib )
SET( SHMLIBSTATIC ${CMAKE_INSTALL_PREFIX}/lib )
SET( SHMINCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include )

FIND_PACKAGE(PkgConfig)
FIND_PACKAGE(Threads REQUIRED)

###############################################################################
# is using the 'pkg-config' tool
#pkg_check_modules(DLT	automotive-dlt)  
##############################################################################
# general stuff



OPTION(BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
OPTION(ENABLE_BUILD_DOC "Set to ON to build Documentation" OFF)

ADD_SUBDIRECTORY( src/CommonAPI )
#ADD_SUBDIRECTORY( Test/Implementation )
#ADD_SUBDIRECTORY( src )


##############################################################################
# installing files

INSTALL(FILES CommonAPI.pc
	DESTINATION /usr/lib/pkgconfig 
	COMPONENT base)


#INSTALL(FILES Config/shm.conf
#	DESTINATION /etc
#	PERMISSIONS OWNER_WRITE OWNER_READ
#	GROUP_READ GROUP_EXECUTE
#	WORLD_READ WORLD_EXECUTE 
#)

CONFIGURE_FILE(commonapi.spec.in commonapi.spec)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/CommonAPI.pc.in ${CMAKE_CURRENT_SOURCE_DIR}/CommonAPI.pc)
CONFIGURE_FILE(CommonAPI-uninstalled.pc.in CommonAPI-uninstalled.pc)
CONFIGURE_FILE(doxygen.cfg.in doxygen.cfg)
#CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in  ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)


