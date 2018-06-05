set(ANGELSCRIPT_BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/AngelScript/sdk")
set(ANGELSCRIPT_INCLUDE_DIRS
  "${ANGELSCRIPT_BASE_DIR}/angelscript/include"
  "${ANGELSCRIPT_BASE_DIR}/add_on")
set(ANGELSCRIPT_LIBRARY "angelscript")
set(ANGELSCRIPT_LIBRARIES ${ANGELSCRIPT_LIBRARY})

add_subdirectory("${ANGELSCRIPT_BASE_DIR}/angelscript/projects/cmake")

target_sources("${ANGELSCRIPT_LIBRARY}" PUBLIC
  "${ANGELSCRIPT_BASE_DIR}/add_on/scriptarray/scriptarray.cpp"
  "${ANGELSCRIPT_BASE_DIR}/add_on/scripthandle/scripthandle.cpp"
  "${ANGELSCRIPT_BASE_DIR}/add_on/scriptstdstring/scriptstdstring.cpp"
  "${ANGELSCRIPT_BASE_DIR}/add_on/scriptstdstring/scriptstdstring_utils.cpp"
  "${ANGELSCRIPT_BASE_DIR}/add_on/weakref/weakref.cpp"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AngelScript DEFAULT_MSG ANGELSCRIPT_LIBRARY)

mark_as_advanced(ANGELSCRIPT_BASE_DIR ANGELSCRIPT_INCLUDE_DIRS ANGELSCRIPT_LIBRARY ANGELSCRIPT_LIBRARIES)
