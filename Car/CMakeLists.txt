if (USE_HIGH_FIDELITY_MODEL)
    add_library(HighFidelity HighFidelity.cpp)
    target_include_directories(HighFidelity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(HighFidelity PRIVATE USE_HIGH_FIDELITY_MODEL)
endif()

