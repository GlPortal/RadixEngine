set(TEST_LIBRARIES ${UnitTestPlusPlus_LIBRARY}  ${RADIX_LIBRARIES})
include_directories(${RADIX_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(tests)

function(test)
  set(TEST_NAME ${ARGV0})
  set(TEST_SOURCES "")
  foreach(argn RANGE 1 ${ARGC})
    list(APPEND TEST_SOURCES "${ARGV${argn}}")
  endforeach(argn)
  add_executable(${TEST_NAME} ${TEST_SOURCES})
  target_link_libraries(${TEST_NAME} ${TEST_LIBRARIES})
  add_dependencies(tests ${TEST_NAME})
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction(test)

test(XmlHelperTest XmlHelperTest.cpp)
test(QuaternionTest ./core/math/QuaternionTest.cpp)
test(MathTest ./core/math/MathTest.cpp)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  test(TextureLoader ./data/texture/TextureLoader.cpp)
  set_target_properties(TextureLoader
    PROPERTIES
    LINK_FLAGS  
    "-Wl,--wrap=fopen"
    #-Wl,--wrap=glGenTextures   \
    #-Wl,--wrap=glBindTexture   \
    #-Wl,--wrap=glPixelStorei   \
    #-Wl,--wrap=glTexImage2D    \
    #-Wl,--wrap=glTexParameteri \
    #-Wl,--wrap=glGenerateMipmap"
  )
endif()

