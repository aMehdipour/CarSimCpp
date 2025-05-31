cmake_minimum_required(VERSION 3.10)
project(VehicleDynamics CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER clang++)

# Find Eigen library
find_package(Eigen3 REQUIRED)
include_directories(
    Car
    Sim
    Integrator
    Utils
    Common
    TireModel
    ThirdParty/nholmann
    Test
    ${EIGEN3_INCLUDE_DIR}
)

# Options to select car model
option(USE_BICYCLE_MODEL "Use Bicycle Model for Car" OFF)
option(USE_HIGH_FIDELITY_MODEL "Use High Fidelity Model for Car" OFF)

# Options to select integrator type
option(USE_EULER_INTEGRATION "Use Euler Integration" OFF)
option(USE_RK4_INTEGRATION "Use Runge-Kutta 4 Integration" OFF)

add_subdirectory(Sim)
add_subdirectory(Car)
add_subdirectory(Integrator)
add_subdirectory(ThirdParty/nholmann)
add_subdirectory(TireModel)
add_subdirectory(Utils)
add_subdirectory(Test)

# Main application executable
add_executable(carsim_main main.cpp)

# Unit test executables
# add_executable(test_integrator Test/testMassSpring.cpp)
add_executable(test_tire_model Test/testTireModel.cpp)
add_executable(test_tire_debug Test/testTireModelDebug.cpp)
add_executable(test_tire_debug_detailed Test/testTireModelDebugDetailed.cpp)
add_executable(test_tire_load Test/testTireModelLoadDependence.cpp)

# Link libraries for main application
target_link_libraries(carsim_main PRIVATE Utils)
target_link_libraries(carsim_main PRIVATE Sim)
target_link_libraries(carsim_main PRIVATE TireModel)
target_link_libraries(carsim_main PRIVATE Integrator)

# Link libraries for test_integrator
# target_link_libraries(test_integrator PRIVATE Sim)
# target_link_libraries(test_integrator PRIVATE TireModel)
# target_link_libraries(test_integrator PRIVATE Utils)
# target_link_libraries(test_integrator PRIVATE Integrator)
# target_link_libraries(test_integrator PRIVATE Test)

# Link libraries for test_tire_model
target_link_libraries(test_tire_model PRIVATE TireModel)
target_link_libraries(test_tire_model PRIVATE Utils)

# Link libraries for test_tire_debug
target_link_libraries(test_tire_debug PRIVATE TireModel)
target_link_libraries(test_tire_debug PRIVATE Utils)

# Link libraries for test_tire_debug_detailed
target_link_libraries(test_tire_debug_detailed PRIVATE TireModel)
target_link_libraries(test_tire_debug_detailed PRIVATE Utils)

# Link libraries for test_tire_load
target_link_libraries(test_tire_load PRIVATE TireModel)
target_link_libraries(test_tire_load PRIVATE Utils)

# Set output directory for test executables
# set_target_properties(test_integrator test_tire_model test_tire_debug test_tire_debug_detailed test_tire_load
set_target_properties( test_tire_model test_tire_debug test_tire_debug_detailed test_tire_load
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests"
)

# Link only the selected car and integrator implementations
# if (USE_BICYCLE_MODEL)
#     target_compile_definitions(vehicle_sim PRIVATE CAR_MODEL=BicycleModel)
#     target_link_libraries(vehicle_sim PRIVATE BicycleModel)
# elseif (USE_HIGH_FIDELITY_MODEL)
#     target_compile_definitions(vehicle_sim PRIVATE CAR_MODEL=HighFidelity)
#     target_link_libraries(vehicle_sim PRIVATE HighFidelity)
# endif()
#
# if (USE_EULER_INTEGRATION)
#     target_compile_definitions(vehicle_sim PRIVATE INTEGRATOR_MODEL=EulerIntegrator)
#     target_link_libraries(vehicle_sim PRIVATE EulerIntegrator)
# elseif (USE_RK4_INTEGRATION)
#     target_compile_definitions(vehicle_sim PRIVATE INTEGRATOR_MODEL=RK4Integrator)
#     target_link_libraries(vehicle_sim PRIVATE RK4Integrator)
# endif()

# Link libraries for tire_test
# target_link_libraries(tire_test PRIVATE TireModel)
# target_link_libraries(tire_test PRIVATE Utils)
