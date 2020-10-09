# Find the FreeImage library.
#
# This module defines
#  FREEIMAGE_FOUND             - True if FREEIMAGE was found.
#  FREEIMAGE_INCLUDE_DIRS      - Include directories for FREEIMAGE headers.
#  FREEIMAGE_LIBRARIES         - Libraries for FREEIMAGE.
#
# To specify an additional directory to search, set FREEIMAGE_ROOT.
#
# Copyright (c) 2010, Ewen Cheslack-Postava
# Based on FindSQLite3.cmake by:
#  Copyright (c) 2006, Jaroslaw Staniek, <js@iidea.pl>
#  Extended by Siddhartha Chaudhuri, 2008.
#
# Redistribution and use is allowed according to the terms of the BSD license.
#

SET(FREEIMAGE_FOUND FALSE)
SET(FREEIMAGE_INCLUDE_DIRS)
SET(FREEIMAGE_LIBRARIES)

IF(WIN32)
	
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/FreeImage3180.zip")
		message(STATUS "Extracting File: FreeImage3180.zip")

		file( ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_SOURCE_DIR}/external/FreeImage3180.zip"
			  DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/external/"
			)	
	endif()
	IF(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/FreeImage")

		set(EXPORT_INIT_SCRIPT "cmd /V:ON /k \" \"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat\" && \
devenv.com ${CMAKE_CURRENT_SOURCE_DIR}\\external\\FreeImage\\FreeImage.2017.sln /Upgrade && \
msbuild ${CMAKE_CURRENT_SOURCE_DIR}\\external\\FreeImage\\FreeImage.2017.sln /t:build /p:WindowsTargetPlatformVersion=${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}\;Configuration=Release\;Platform=x64 && exit\" ")

		file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/external/FreeImage/init.bat" ${EXPORT_INIT_SCRIPT})
		message(STATUS "Building the project: FreeImage")
		execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/external/FreeImage/init.bat" OUTPUT_QUIET)		
			
		file(GLOB_RECURSE FREEIMAGE_LIBRARIES
		  "${CMAKE_CURRENT_SOURCE_DIR}/external/FreeImage/*.lib"
		)
		
		set(FREEIMAGE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/FreeImage/Source" "${CMAKE_CURRENT_SOURCE_DIR}/external/FreeImage/Wrapper/FreeImagePlus/dist/x64")
	ENDIF()
	
	include(FindPackageHandleStandardArgs)
	find_package_handle_standard_args(FreeImage FREEIMAGE_LIBRARIES FREEIMAGE_INCLUDE_DIR)

ELSE()
	SET(SEARCH_PATHS
	  $ENV{ProgramFiles}/freeimage/include
	  $ENV{SystemDrive}/freeimage/include
	  $ENV{ProgramFiles}/freeimage
	  $ENV{SystemDrive}/freeimage
	  )
	IF(FREEIMAGE_ROOT)
	  SET(SEARCH_PATHS
		${FREEIMAGE_ROOT}
		${FREEIMAGE_ROOT}/include
		${SEARCH_PATHS}
		)
	ENDIF()

	FIND_PATH(FREEIMAGE_INCLUDE_DIRS
	  NAMES FreeImage.h
	  PATHS ${SEARCH_PATHS}
	  NO_DEFAULT_PATH)
	IF(NOT FREEIMAGE_INCLUDE_DIRS)  # now look in system locations
	  FIND_PATH(FREEIMAGE_INCLUDE_DIRS NAMES FreeImage.h)
	ENDIF(NOT FREEIMAGE_INCLUDE_DIRS)

	SET(FREEIMAGE_LIBRARY_DIRS)
	IF(FREEIMAGE_ROOT)
	  SET(FREEIMAGE_LIBRARY_DIRS ${FREEIMAGE_ROOT})
	  IF(EXISTS "${FREEIMAGE_ROOT}/lib")
		SET(FREEIMAGE_LIBRARY_DIRS ${FREEIMAGE_LIBRARY_DIRS} ${FREEIMAGE_ROOT}/lib)
	  ENDIF()
	  IF(EXISTS "${FREEIMAGE_ROOT}/lib/static")
		SET(FREEIMAGE_LIBRARY_DIRS ${FREEIMAGE_LIBRARY_DIRS} ${FREEIMAGE_ROOT}/lib/static)
	  ENDIF()
	ENDIF()

	# FREEIMAGE
	# Without system dirs
	FIND_LIBRARY(FREEIMAGE_LIBRARY
	  NAMES freeimage
	  PATHS ${FREEIMAGE_LIBRARY_DIRS}
	  NO_DEFAULT_PATH
	  )
	IF(NOT FREEIMAGE_LIBRARY)  # now look in system locations
	  FIND_LIBRARY(FREEIMAGE_LIBRARY NAMES freeimage)
	ENDIF(NOT FREEIMAGE_LIBRARY)

	SET(FREEIMAGE_LIBRARIES)
	IF(FREEIMAGE_LIBRARY)
	  SET(FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})
	ENDIF()

	IF(FREEIMAGE_INCLUDE_DIRS AND FREEIMAGE_LIBRARIES)
	  SET(FREEIMAGE_FOUND TRUE)
	  IF(NOT FREEIMAGE_FIND_QUIETLY)
		MESSAGE(STATUS "Found FreeImage: headers at ${FREEIMAGE_INCLUDE_DIRS}, libraries at ${FREEIMAGE_LIBRARY_DIRS} :: ${FREEIMAGE_LIBRARIES}")
	  ENDIF(NOT FREEIMAGE_FIND_QUIETLY)
	ELSE(FREEIMAGE_INCLUDE_DIRS AND FREEIMAGE_LIBRARIES)
	  SET(FREEIMAGE_FOUND FALSE)
	  IF(FREEIMAGE_FIND_REQUIRED)
		MESSAGE(STATUS "FreeImage not found")
	  ENDIF(FREEIMAGE_FIND_REQUIRED)
	ENDIF(FREEIMAGE_INCLUDE_DIRS AND FREEIMAGE_LIBRARIES)
ENDIF(WIN32)

MARK_AS_ADVANCED(FREEIMAGE_INCLUDE_DIRS FREEIMAGE_LIBRARIES)
