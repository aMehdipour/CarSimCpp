file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
add_library(Utils ${SOURCES})
target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
