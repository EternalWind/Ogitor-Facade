
cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

project(OgCaelum)


set(HEADER_FILES
    CaelumEditor.h
)
set(SOURCE_FILES
    CaelumEditor.cpp
)


SET(OgCaelum_VERSION 0)
SET(OgCaelum_VERSION_MAJOR 1)

include_directories (include)
# Terrain Scene Manager
#include_directories ( ${OGITOR_SM_INCLUDES} )
include_directories(${DEPENDENCIES_INCLUDES})
include_directories(${OGITOR_INCLUDES})
MESSAGE(STATUS "${OGITOR_INCLUDES}" )

include_directories(.)
LINK_DIRECTORIES( ${OGITOR_LIBPATH} )
LINK_DIRECTORIES( ${DEPENDENCIES_LIBPATH} )

link_libraries( ${OGRE_LIBRARIES} Ogitor Caelum)
add_library(OgCaelum SHARED ${HEADER_FILES} ${SOURCE_FILES}  )
#target_link_libraries(OgreMain Ogitor ETL)
SET(LIBRARY_OUTPUT_PATH ../lib)

set_target_properties(OgCaelum PROPERTIES
        INSTALL_RPATH ${OGRE_INSTALL_DIR}/lib/OGRE  )

set_target_properties(OgCaelum PROPERTIES 
    SOLUTION_FOLDER Plugins) 

set_target_properties(OgCaelum PROPERTIES DEBUG_POSTFIX "" VERSION ${OgCaelum_VERSION} SOVERSION ${OgCaelum_VERSION_MAJOR})

set_target_properties(OgCaelum PROPERTIES
COMPILE_DEFINITIONS PLUGIN_EXPORT
)

IF(WIN32)
    add_custom_command( TARGET OgCaelum PRE_BUILD
        COMMAND if not exist ..\\..\\RunPath\\Plugins mkdir ..\\..\\RunPath\\Plugins)
    add_custom_command( TARGET OgCaelum POST_BUILD
        COMMAND copy \"$(TargetPath)\" ..\\..\\RunPath\\Plugins & if exist ..\\..\\RunPath\\Plugins\\OgCaelum_d.dll move ..\\..\\RunPath\\Plugins\\OgCaelum_d.dll ..\\..\\RunPath\\Plugins\\debug)
ENDIF(WIN32)

IF(UNIX AND NOT APPLE)
    add_custom_command( TARGET OgCaelum PRE_BUILD
        COMMAND mkdir -p ../../RunPath/Plugins )
    add_custom_command( TARGET OgCaelum POST_BUILD
        COMMAND cp -r ../lib/libOgCaelum.* ../../RunPath/Plugins/ )
ENDIF(UNIX AND NOT APPLE)

IF(WIN32)
install(TARGETS OgCaelum
    RUNTIME DESTINATION Plugins
    CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
ENDIF()
