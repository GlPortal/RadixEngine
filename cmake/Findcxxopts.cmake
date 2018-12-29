set(CXXOPTS_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/cxxopts")

add_subdirectory("${CXXOPTS_BASE_DIR}")
set(CXXOPTS_INCLUDE_DIRS
  "${CXXOPTS_BASE_DIR}/include/")
set(CXXOPTS_LIBRARY "cxxopts")
set(CXXOPTS_LIBRARIES ${CXXOPTS_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(cxxopts DEFAULT_MSG CXXOPTS_LIBRARY)

mark_as_advanced(CXXOPTS_BASE_DIR CXXOPTS_INCLUDE_DIRS CXXOPTS_LIBRARY CXXOPTS_LIBRARIES)
