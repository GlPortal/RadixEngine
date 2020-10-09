IF(WIN32)
	message(STATUS "Building the project: VHACD")
	execute_process(COMMAND cmake -S "${CMAKE_CURRENT_SOURCE_DIR}/external/v-hacd/src" -B "${CMAKE_CURRENT_SOURCE_DIR}/external/v-hacd/build" -A x64 -D NO_OPENCL=ON -D NO_OPENMP=ON OUTPUT_QUIET)
	execute_process(COMMAND cmake --build "${CMAKE_CURRENT_SOURCE_DIR}/external/v-hacd/build/VHACD_Lib" --target "vhacd" --config "Release" OUTPUT_QUIET)
	
	set(VHACD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/v-hacd/src/VHACD_Lib/inc" CACHE STRING "Location of VHACD includes")
	FIND_LIBRARY(VHACD_LIBRARIES
				 NAMES vhacd.lib
				 PATHS ${CMAKE_CURRENT_SOURCE_DIR}/external/v-hacd/build/VHACD_Lib/Release
				)
ELSE()
	set(VHACD_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/vhacd-lib")
	set(VHACD_INCLUDE_DIR "${VHACD_ROOT_DIR}/public")
	set(VHACD_INCLUDE_DIRS ${VHACD_INCLUDE_DIR})
	set(VHACD_LIBRARY "VHACD_LIB")
	set(VHACD_LIBRARIES ${VHACD_LIBRARY})

	set(NO_OPENCL ON)
	set(NO_OPENMP ON)
	add_subdirectory("${VHACD_ROOT_DIR}")
ENDIF()

include(FindPackageHandleStandardArgs)
if(WIN32)
find_package_handle_standard_args(VHACD DEFAULT_MSG VHACD_LIBRARIES VHACD_INCLUDE_DIR)
ELSE()
find_package_handle_standard_args(VHACD DEFAULT_MSG VHACD_LIBRARIES VHACD_INCLUDE_DIR VHACD_INCLUDE_DIRS)
ENDIF(WIN32)
mark_as_advanced(VHACD_LIBRARY VHACD_LIBRARIES VHACD_INCLUDE_DIR VHACD_INCLUDE_DIRS)
