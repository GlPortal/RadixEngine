cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
set(RADIX_LIBRARY RadixEngine)

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp")
	execute_process(COMMAND git submodule add https://github.com/assimp/assimp.git external/assimp
					WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/bullet")
execute_process(COMMAND git submodule add https://github.com/bulletphysics/bullet3.git external/bullet
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml2")
execute_process(COMMAND git submodule add https://github.com/leethomason/tinyxml2.git external/tinyxml2
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/v-hacd")
execute_process(COMMAND git submodule add https://github.com/kmammou/v-hacd.git external/v-hacd
				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

#set(EXPORT_INIT_SCRIPT "git submodule add https://github.com/assimp/assimp.git external/assimp && \
#git submodule add https://github.com/bulletphysics/bullet3.git external/bullet && \
#git submodule add https://github.com/leethomason/tinyxml2.git external/tinyxml2 && \
#git submodule add https://github.com/kmammou/v-hacd.git external/v-hacd")

#file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/getMissingSubmodules.bat" ${EXPORT_INIT_SCRIPT})
#execute_process(COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/getMissingSubmodules.bat"
#				WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if (COVERAGE)
  message("-- Coverage Enabled")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  --coverage -fprofile-arcs -ftest-coverage")
endif ()

if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type")
endif()

project(${RADIX_LIBRARY} CXX C)
set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(DEFINED WIN32)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MT> #----|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()

#### UTILITY ####

set(RADIX_DEFINITIONS "")
function(radix_add_exported_definition)
  add_definitions("-D${ARGV0}")
  list(APPEND RADIX_DEFINITIONS "${ARGV0}")
  set(RADIX_DEFINITIONS "${RADIX_DEFINITIONS}" PARENT_SCOPE)
endfunction(radix_add_exported_definition)

#### COMPILE-TIME SETTINGS ####

set(RADIX_GL_LOADER "glad" CACHE STRING "OpenGL loader to use. One of 'libepoxy', 'glad'")

if ("${RADIX_GL_LOADER}" STREQUAL "libepoxy")
  set(RADIX_GL_USE_LIBEPOXY TRUE)
  radix_add_exported_definition("RADIX_GL_USE_LIBEPOXY")
endif()
if ("${RADIX_GL_LOADER}" STREQUAL "glad")
  set(RADIX_GL_USE_GLAD TRUE)
  radix_add_exported_definition("RADIX_GL_USE_GLAD")
endif()

#### PLATFORM-SPECIFIC FIXES ####

if(DEFINED WIN32)
  add_definitions("-D_USE_MATH_DEFINES")
endif()

#### LIBRARIES ####

set(RADIX_INCLUDE_DIRS "")

find_package(OpenGL REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
list(APPEND RADIX_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/external/stx/include)
list(APPEND RADIX_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/radix)

set(RADIX_LIBRARIES ${OPENGL_gl_LIBRARY})

find_package(PkgConfig REQUIRED)

if (RADIX_GL_USE_LIBEPOXY)
  pkg_search_module(EPOXY REQUIRED epoxy)
  list(APPEND RADIX_INCLUDE_DIRS ${EPOXY_INCLUDE_DIRS})
  list(APPEND RADIX_LIBRARIES ${EPOXY_LIBRARIES})
elseif (RADIX_GL_USE_GLAD)
  set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glad")
  set(GLAD_SRC "${GLAD_INCLUDE_DIR}/glad.c")
  add_library("GLAD" ${GLAD_SRC})
  list(APPEND RADIX_INCLUDE_DIRS ${GLAD_INCLUDE_DIR})
  list(APPEND RADIX_LIBRARIES GLAD)
endif()

find_package(Assimp REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIRS})
list(APPEND RADIX_LIBRARIES ${ASSIMP_LIBRARIES} )

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIRS})
list(APPEND RADIX_LIBRARIES ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARIES} )

find_package(TinyXML2 2.0.0 REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${TINYXML2_INCLUDE_DIR})
list(APPEND RADIX_LIBRARIES ${TINYXML2_LIBRARIES})
add_definitions("-DTINYXML2_MAJOR_VERSION=${TINYXML2_MAJOR_VERSION}")
add_definitions("-DTINYXML2_MINOR_VERSION=${TINYXML2_MINOR_VERSION}")
add_definitions("-DTINYXML2_PATCH_VERSION=${TINYXML2_PATCH_VERSION}")
add_definitions("-DTINYXML2_VERSION_CODE=${TINYXML2_VERSION_CODE}")

find_package(json11 REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${JSON11_INCLUDE_DIR})
list(APPEND RADIX_LIBRARIES ${JSON11_LIBRARY})

find_package(Catch REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${CATCH_INCLUDE_DIR})

find_package(Linenoise REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${LINENOISE_INCLUDE_DIR})


IF(WIN32)
	message(STATUS "Building the project: Bullet")
	execute_process(COMMAND cmake -S "${CMAKE_CURRENT_SOURCE_DIR}/external/bullet" -B "${CMAKE_CURRENT_SOURCE_DIR}/external/bullet/build" -A x64 OUTPUT_QUIET)
	execute_process(COMMAND cmake --build "${CMAKE_CURRENT_SOURCE_DIR}/external/bullet/build" --target "BulletCollision" "BulletDynamics" "LinearMath" --config "Release" OUTPUT_QUIET)
	
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/bullet/src")
		file(RENAME "${CMAKE_CURRENT_SOURCE_DIR}/external/bullet/src" "${CMAKE_CURRENT_SOURCE_DIR}/external/bullet/bullet" )
	endif()
	
	find_library(BULLET_DYNAMICS_LIBRARY
				 NAMES BulletDynamics.lib
				 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/external/bullet/build/lib/Release
				)
			  
	find_library(BULLET_COLLISION_LIBRARY
				 NAMES BulletCollision.lib
				 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/external/bullet/build/lib/Release
				)
			  
	find_library(BULLET_MATH_LIBRARY
				 NAMES LinearMath.lib
				 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/external/bullet/build/lib/Release
				)
	
	set(BULLET_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/bullet" "${CMAKE_CURRENT_SOURCE_DIR}/external/bullet/bullet" CACHE STRING "Location of Bullet includes")
	list(APPEND RADIX_INCLUDE_DIRS ${BULLET_INCLUDE_DIR})
	list(APPEND RADIX_LIBRARIES ${BULLET_DYNAMICS_LIBRARY} ${BULLET_COLLISION_LIBRARY} ${BULLET_MATH_LIBRARY} )
	
ELSE()
	find_package(Bullet 2.82 REQUIRED)
	list(APPEND RADIX_INCLUDE_DIRS ${BULLET_INCLUDE_DIR})
	list(APPEND RADIX_LIBRARIES ${BULLET_LIBRARIES})
ENDIF()


find_package(VHACD REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${VHACD_INCLUDE_DIR})
list(APPEND RADIX_LIBRARIES ${VHACD_LIBRARY})

find_package(FreeImage REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${FREEIMAGE_INCLUDE_DIR})
list(APPEND RADIX_LIBRARIES ${FREEIMAGE_LIBRARIES})

find_package(AngelScript REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${ANGELSCRIPT_INCLUDE_DIRS})
list(APPEND RADIX_LIBRARIES ${ANGELSCRIPT_LIBRARY})

find_package(easy_profiler REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${EASY_PROFILER_INCLUDE_DIR})
set(RADIX_ENABLE_PROFILER OFF CACHE BOOL "Enable Radix's profiler")
if (RADIX_ENABLE_PROFILER)
  list(APPEND RADIX_LIBRARIES ${EASY_PROFILER_LIBRARIES})
else ()
  radix_add_exported_definition("DISABLE_EASY_PROFILER")
endif ()

set(PORTAL_TEST_LIBRARIES ${RADIX_LIBRARIES})

list(APPEND RADIX_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/external/serine/include")

#### ENGINE ####

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_definitions("-Wall")
endif ()

find_package(RadixEntity REQUIRED)
list(APPEND RADIX_INCLUDE_DIRS ${RADIXENTITY_INCLUDE_DIR})
list(APPEND RADIX_LIBRARIES ${RADIXENTITY_LIBRARIES})

set(RADIX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
list(APPEND RADIX_INCLUDE_DIRS "${RADIX_INCLUDE_DIR}")

include_directories(${RADIX_INCLUDE_DIRS})

add_subdirectory(source)

add_library(${RADIX_LIBRARY} STATIC ${RADIX_SOURCES})

# Export
list(APPEND RADIX_LIBRARIES ${RADIX_LIBRARY})

if (RADIX_EXPORT_CMAKE_PACKAGE_VARS)
  set(RADIX_LIBRARY "${RADIX_LIBRARY}" PARENT_SCOPE)
  set(RADIX_LIBRARIES "${RADIX_LIBRARIES}" PARENT_SCOPE)
  set(RADIX_INCLUDE_DIR "${RADIX_INCLUDE_DIR}" PARENT_SCOPE)
  set(RADIX_INCLUDE_DIRS "${RADIX_INCLUDE_DIRS}" PARENT_SCOPE)
  set(RADIX_DEFINITIONS "${RADIX_DEFINITIONS}" PARENT_SCOPE)
endif ()

#### UNIT TESTING ####

include_directories(${RADIX_INCLUDE_DIRS} ${UnitTestPlusPlus_INCLUDE_DIRS})
enable_testing()
add_subdirectory(tests)

if (CMAKE_VERSION VERSION_GREATER 2.8.5)
  include(FeatureSummary)
  feature_summary(WHAT ALL)
endif ()
