# - Locate SDL_mixer library
# This module defines:
#  SDL2_MIXER_LIBRARIES, the name of the library to link against
#  SDL2_MIXER_INCLUDE_DIRS, where to find the headers
#  SDL2_MIXER_FOUND, if false, do not try to link against
#  SDL2_MIXER_VERSION_STRING - human-readable string containing the version of SDL_mixer
#
# For backward compatiblity the following variables are also set:
#  SDLMIXER_LIBRARY (same value as SDL2_MIXER_LIBRARIES)
#  SDLMIXER_INCLUDE_DIR (same value as SDL2_MIXER_INCLUDE_DIRS)
#  SDLMIXER_FOUND (same value as SDL2_MIXER_FOUND)
#
# $SDLDIR is an environment variable that would
# correspond to the ./configure --prefix=$SDLDIR
# used in building SDL.
#
# Created by Eric Wing. This was influenced by the FindSDL.cmake
# module, but with modifications to recognize OS X frameworks and
# additional Unix paths (FreeBSD, etc).

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
# Copyright 2012 Benjamin Eikel
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(SDL2_ARCH_64 TRUE)
  set(SDL2_PROCESSOR_ARCH "x64")
else()
  set(SDL2_ARCH_64 FALSE)
  set(SDL2_PROCESSOR_ARCH "x86")
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

IF(WIN32)

	set(url "https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-2.0.4.zip")
	set(filename "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_mixer-2.0.4.zip")
	
	if(NOT EXISTS ${filename})
		message(STATUS "Downloading SDL2-2.0.12.zip...")
		file(	DOWNLOAD 		${url} ${filename}
				TIMEOUT 		60  # seconds
				#EXPECTED_HASH 	${hash_type}=${hash}
				TLS_VERIFY 		ON
				SHOW_PROGRESS
		)
	endif()

	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_mixer-2.0.4.zip")
		message(STATUS "Extracting File: SDL2_mixer-2.0.4.zip")

		file( ARCHIVE_EXTRACT INPUT "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_mixer-2.0.4.zip"
			  DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/external/"
			)	
	endif()
	
	set(EXPORT_INIT_SCRIPT "cmd /V:ON /k \" \"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat\" && \
devenv.com ${CMAKE_CURRENT_SOURCE_DIR}\\external\\SDL2_mixer-2.0.4\\VisualC\\SDL_mixer.sln /Upgrade && \
set INCLUDE=!INCLUDE!${CMAKE_CURRENT_SOURCE_DIR}\\external\\SDL2-2.0.12\\build\\include\;\
${CMAKE_CURRENT_SOURCE_DIR}\\external\\SDL2-2.0.12\\include\;\ && \
set LIB=!LIB!${CMAKE_CURRENT_SOURCE_DIR}\\external\\SDL2-2.0.12\\build\\Release\; && \
devenv.com /useenv ${CMAKE_CURRENT_SOURCE_DIR}\\external\\SDL2_mixer-2.0.4\\VisualC\\SDL_mixer.sln /build Release^|x64 /project SDL2_mixer && exit\" ")

	file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_mixer-2.0.4/init.bat" ${EXPORT_INIT_SCRIPT})
	message(STATUS "Building the project: SDL2_mixer-2.0.4")
	execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_mixer-2.0.4/init.bat" OUTPUT_QUIET)
	
	set(SDL2_MIXER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_mixer-2.0.4" CACHE STRING "Location of SDL2 Mixer includes")
	FIND_LIBRARY(SDL2_MIXER_LIBRARY
				 NAMES SDL2_mixer.lib
				 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/external/SDL2_mixer-2.0.4/VisualC/x64/Release
			  )
	
	unset(filename)
	unset(url)

ELSE()
	SET(SDL2_SEARCH_PATHS
		~/Library/Frameworks
		/Library/Frameworks
		/usr/local
		/usr
		/sw # Fink
		/opt/local # DarwinPorts
		/opt/csw # Blastwave
		/opt
	)

	if(NOT SDL2_MIXER_INCLUDE_DIR AND SDL2MIXER_INCLUDE_DIR)
	  set(SDL2_MIXER_INCLUDE_DIR ${SDL2MIXER_INCLUDE_DIR} CACHE PATH "directory cache
	entry initialized from old variable name")
	endif()
	find_path(SDL2_MIXER_INCLUDE_DIR SDL_mixer.h
	  HINTS
		ENV SDL2MIXERDIR
		ENV SDL2DIR
	  PATH_SUFFIXES include include/SDL2 SDL2
	  PATHS ${SDL2_SEARCH_PATHS}
	)

	if(NOT SDL2_MIXER_LIBRARY AND SDL2MIXER_LIBRARY)
	  set(SDL2_MIXER_LIBRARY ${SDL2MIXER_LIBRARY} CACHE FILEPATH "file cache entry
	initialized from old variable name")
	endif()
	find_library(SDL2_MIXER_LIBRARY
	  NAMES SDL2_mixer
	  HINTS
		ENV SDL2MIXERDIR
		ENV SDL2DIR
	  PATH_SUFFIXES lib64 lib lib/${SDL2_PROCESSOR_ARCH}
	  PATHS ${SDL2_SEARCH_PATHS}
	)

	if(SDL2_MIXER_INCLUDE_DIR AND EXISTS "${SDL2_MIXER_INCLUDE_DIR}/SDL_mixer.h")
	  file(STRINGS "${SDL2_MIXER_INCLUDE_DIR}/SDL_mixer.h" SDL2_MIXER_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL_MIXER_MAJOR_VERSION[ \t]+[0-9]+$")
	  file(STRINGS "${SDL2_MIXER_INCLUDE_DIR}/SDL_mixer.h" SDL2_MIXER_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL_MIXER_MINOR_VERSION[ \t]+[0-9]+$")
	  file(STRINGS "${SDL2_MIXER_INCLUDE_DIR}/SDL_mixer.h" SDL2_MIXER_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL_MIXER_PATCHLEVEL[ \t]+[0-9]+$")
	  string(REGEX REPLACE "^#define[ \t]+SDL_MIXER_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_MIXER_VERSION_MAJOR "${SDL2_MIXER_VERSION_MAJOR_LINE}")
	  string(REGEX REPLACE "^#define[ \t]+SDL_MIXER_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_MIXER_VERSION_MINOR "${SDL2_MIXER_VERSION_MINOR_LINE}")
	  string(REGEX REPLACE "^#define[ \t]+SDL_MIXER_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL2_MIXER_VERSION_PATCH "${SDL2_MIXER_VERSION_PATCH_LINE}")
	  set(SDL2_MIXER_VERSION_STRING ${SDL2_MIXER_VERSION_MAJOR}.${SDL2_MIXER_VERSION_MINOR}.${SDL2_MIXER_VERSION_PATCH})
	  unset(SDL2_MIXER_VERSION_MAJOR_LINE)
	  unset(SDL2_MIXER_VERSION_MINOR_LINE)
	  unset(SDL2_MIXER_VERSION_PATCH_LINE)
	  unset(SDL2_MIXER_VERSION_MAJOR)
	  unset(SDL2_MIXER_VERSION_MINOR)
	  unset(SDL2_MIXER_VERSION_PATCH)
	endif()


ENDIF(WIN32)

set(SDL2_MIXER_LIBRARIES ${SDL2_MIXER_LIBRARY})
set(SDL2_MIXER_INCLUDE_DIRS ${SDL2_MIXER_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_mixer
                                  REQUIRED_VARS SDL2_MIXER_LIBRARIES SDL2_MIXER_INCLUDE_DIRS
                                  VERSION_VAR SDL2_MIXER_VERSION_STRING)

# for backward compatiblity
set(SDL2MIXER_LIBRARY ${SDL2_MIXER_LIBRARIES})
set(SDL2MIXER_INCLUDE_DIR ${SDL2_MIXER_INCLUDE_DIRS})
set(SDL2MIXER_FOUND ${SDL2_MIXER_FOUND})

mark_as_advanced(SDL2_MIXER_LIBRARY SDL2_MIXER_INCLUDE_DIR)
