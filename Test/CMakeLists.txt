file(GLOB_RECURSE SOURCES "*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*testMassSpring\\.cpp$")
list(FILTER SOURCES EXCLUDE REGEX ".*test_integrator\\.cpp$")

file(GLOB_RECURSE HEADERS "*.h")
add_library(Test ${SOURCES})
target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

