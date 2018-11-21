set(CPPFS_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/cppfs")
list(APPEND CMAKE_MODULE_PATH ${CPPFS_BASE_DIR}/cmake)
#include(GenerateExportHeader)
#include(Custom)
#include(HealthCheck)
add_subdirectory("${CPPFS_BASE_DIR}")
set(CPPFS_INCLUDE_DIRS
  "${CPPFS_BASE_DIR}/source/cppfs/include/cppfs")
set(CPPFS_LIBRARY "cppfs")
set(CPPFS_LIBRARIES ${CPPFS_LIBRARY})

#target_sources("${CPPFS_LIBRARY}" PUBLIC
#  "${CPPFS_BASE_DIR}/source/cppfs/source"
#)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(cppfs DEFAULT_MSG CPPFS_LIBRARY)

mark_as_advanced(CPPFS_BASE_DIR CPPFS_INCLUDE_DIRS CPPFS_LIBRARY CPPFS_LIBRARIES)
