file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_library(Test ${SOURCES})
target_include_directories(Test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

