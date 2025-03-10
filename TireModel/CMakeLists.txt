file(GLOB_RECURSE SOURCES "*.cpp")
add_library(TireModel ${SOURCES})
target_include_directories(TireModel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
