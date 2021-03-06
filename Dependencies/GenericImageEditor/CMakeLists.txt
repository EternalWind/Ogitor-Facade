cmake_minimum_required(VERSION 2.6)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)
cmake_policy(SET CMP0003 NEW)

project(GenericImageEditor)

set(QT_MIN_VERSION "4.5.0")
find_package(Qt4 COMPONENTS QtSvg REQUIRED)

if(NOT QT4_FOUND)
	MESSAGE(SEND_ERROR "Failed to find Qt 4.5 or greater.")
else(NOT QT4_FOUND)
	include(${QT_USE_FILE})
	set(CMAKE_PACKAGE_QTGUI TRUE)

	file(GLOB UI_FILES *.ui)

	set(HEADER_FILES
		./include/genericimageeditor.hxx
		./include/iimageeditorcodec.hxx
		./include/genericimageeditorcodec.hxx
		./include/heightimageeditorcodec.hxx
	)

	set(SOURCE_FILES
		./src/genericimageeditor.cpp
		./src/genericimageeditorcodec.cpp
		./src/heightimageeditorcodec.cpp
	)

	SET(MOC_SRCS2
		./include/genericimageeditor.hxx
		./include/iimageeditorcodec.hxx
		./include/genericimageeditorcodec.hxx
		./include/heightimageeditorcodec.hxx
	)

	qt4_wrap_cpp(MOC_SRCS ${MOC_SRCS2})
	qt4_wrap_ui(UI_SRCS ${UI_FILES})

	source_group("Moc Files" FILES ${MOC_SRCS})
	source_group("Ui Files" FILES ${UI_FILES})

	# if we don't include this CMake will not include ui headers properly:
	include_directories(${CMAKE_CURRENT_BINARY_DIR})

	set(SRCS ${HEADER_FILES} ${SOURCE_FILES} ${MOC_SRCS} ${UI_FILES})

	set(GenericImageEditor_VERSION 0)
	set(GenericImageEditor_VERSION_MAJOR 1)

	include_directories(include)

	include_directories(.)
	include_directories(${OGITOR_INCLUDES})
        include_directories(../OFS/include)

	link_libraries(${QT_QTMAIN_LIBRARY} ${QT_LIBRARIES} OFS Ogitor)
	add_library(GenericImageEditor SHARED ${SRCS})
	set(LIBRARY_OUTPUT_PATH ../lib)

        add_dependencies(GenericImageEditor OFS)

#	set_target_properties(GenericImageEditor PROPERTIES INSTALL_RPATH ${OGRE_INSTALL_DIR}/lib/OGRE)
	set_target_properties(GenericImageEditor PROPERTIES VERSION ${GenericImageEditor_VERSION} SOVERSION ${GenericImageEditor_VERSION_MAJOR})
	set_target_properties(GenericImageEditor PROPERTIES COMPILE_DEFINITIONS GENERICIMAGEEDITOR_EXPORT)

	if(UNIX AND NOT APPLE)
		add_custom_command(TARGET GenericImageEditor PRE_BUILD
			COMMAND mkdir -p ../../RunPath/bin)
		add_custom_command(TARGET GenericImageEditor POST_BUILD
			COMMAND cp -r ../lib/libGenericImageEditor_Ogitor.* ../../RunPath/bin/)
	endif(UNIX AND NOT APPLE)

	if(WIN32)
		add_custom_command(TARGET GenericImageEditor PRE_BUILD
			COMMAND if not exist ..\\..\\RunPath\\bin mkdir ..\\..\\RunPath\\bin)
		add_custom_command(TARGET GenericImageEditor POST_BUILD
			COMMAND copy \"$(TargetPath)\" ..\\..\\RunPath\\bin)

		install(TARGETS GenericImageEditor
			RUNTIME DESTINATION bin
			CONFIGURATIONS Release RelWithDebInfo MinSizeRel)
	endif(WIN32)

IF(UNIX AND NOT APPLE)

set_target_properties( GenericImageEditor PROPERTIES OUTPUT_NAME "GenericImageEditor_Ogitor" VERSION
${OGITOR_VERSION_STRING} SOVERSION ${abi_version} )

install(TARGETS GenericImageEditor
    LIBRARY DESTINATION lib
    CONFIGURATIONS Release RelWithDebInfo MinSizeRel)

ENDIF(UNIX AND NOT APPLE)

endif(NOT QT4_FOUND)

# vim: set sw=2 ts=2 noet:
