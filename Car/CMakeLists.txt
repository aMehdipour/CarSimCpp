if (USE_BICYCLE_MODEL)
    add_library(BicycleModel BicycleModel.cpp)
    target_include_directories(BicycleModel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(BicycleModel PRIVATE USE_BICYCLE_MODEL)
endif()

if (USE_HIGH_FIDELITY_MODEL)
    add_library(HighFidelity HighFidelity.cpp)
    target_include_directories(HighFidelity PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(HighFidelity PRIVATE USE_HIGH_FIDELITY_MODEL)
endif()

