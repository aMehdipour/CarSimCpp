if (USE_EULER_INTEGRATION)
    add_library(EulerIntegrator EulerIntegrator.cpp)
    target_include_directories(EulerIntegrator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(EulerIntegrator PRIVATE USE_EULER_INTEGRATION)
endif()

if (USE_RK4_INTEGRATION)
    add_library(RK4Integrator RK4Integrator.cpp)
    target_include_directories(RK4Integrator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_definitions(RK4Integrator PRIVATE USE_RK4_INTEGRATION)
endif()

