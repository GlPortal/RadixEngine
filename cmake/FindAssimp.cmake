# - Try to find Assimp
# Once done, this will define
#
#  ASSIMP_FOUND - system has Assimp
#  ASSIMP_INCLUDE_DIRS - the Assimp include directories
#  ASSIMP_LIBRARIES - link these to use Assimp
IF(WIN32)
	message(STATUS "Compiling Assimp...")
	execute_process(COMMAND cmake -S "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp" -B "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/build" -A x64 OUTPUT_QUIET)
	execute_process(COMMAND cmake --build "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/build" --target "assimp" --config "Release" OUTPUT_QUIET)
	

	set(ASSIMP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/build/include" "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include" CACHE STRING "Location of Assimp includes")
	FIND_LIBRARY(ASSIMP_LIBRARY
				 NAMES assimp-vc142-mt.lib
				 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/build/lib/Release
			  )

ELSE()

	FIND_PATH( ASSIMP_INCLUDE_DIR assimp/mesh.h
	/usr/include
	/usr/local/include
	/opt/local/include
	)

	FIND_LIBRARY( ASSIMP_LIBRARY assimp
	/usr/lib64
	/usr/lib
	/usr/local/lib
	/opt/local/lib
	)
ENDIF()

IF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)
  SET( ASSIMP_FOUND TRUE )
  SET( ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR} )
  SET( ASSIMP_LIBRARIES ${ASSIMP_LIBRARY} )
ENDIF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY)

# handle REQUIRED and QUIET options
include ( FindPackageHandleStandardArgs )

find_package_handle_standard_args ( Assimp 
  REQUIRED_VARS ASSIMP_LIBRARY ASSIMP_INCLUDE_DIR
)
