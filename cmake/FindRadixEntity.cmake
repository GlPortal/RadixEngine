set(RADIXENTITY_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/RadixEntity")
set(RADIXENTITY_INCLUDE_DIR "${RADIXENTITY_ROOT_DIR}/include/")
set(RADIXENTITY_INCLUDE_DIRS ${RADIXENTITY_INCLUDE_DIR})
set(RADIXENTITY_LIBRARY "RadixEntity")
set(RADIXENTITY_LIBRARIES ${RADIXENTITY_LIBRARY})

add_subdirectory("${RADIXENTITY_ROOT_DIR}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(${RADIXENTITY_LIBRARY} DEFAULT_MSG RADIXENTITY_LIBRARIES RADIXENTITY_INCLUDE_DIR RADIXENTITY_INCLUDE_DIRS)

mark_as_advanced(RADIXENTITY_LIBRARY RADIXENTITY_LIBRARIES RADIXENTITY_INCLUDE_DIR RADIXENTITY_INCLUDE_DIRS)
