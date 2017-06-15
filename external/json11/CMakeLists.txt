cmake_minimum_required(VERSION 2.6)

if(CMAKE_VERSION VERSION_LESS "3")
  add_definitions(-std=c++11)
else()
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

project(json11 CXX)
add_library(json11 STATIC json11.cpp)
target_compile_options(json11 PRIVATE -fno-rtti -fno-exceptions -Wall)

# Set warning flags, which may vary per platform
include(CheckCXXCompilerFlag)
set(_possible_warnings_flags /W4 /WX -Wextra -Werror)
foreach(_warning_flag in ${_possible_warnings_flags})
  CHECK_CXX_COMPILER_FLAG(_warning_flag _flag_supported)
  if(${_flag_supported})
    target_compile_options(json11 PRIVATE ${_warning_flag})
  endif()
endforeach()
