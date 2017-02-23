cmake_minimum_required(VERSION 2.6)

project(GLAD CXX)

file(GLOB_RECURSE GLAD_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}" "*.cpp")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_library(GLAD STATIC ${GLAD_SOURCES})