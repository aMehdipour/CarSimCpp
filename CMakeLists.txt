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

add_executable(vehicle_sim testMassSpring.cpp)

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

target_link_libraries(vehicle_sim PRIVATE Sim)
target_link_libraries(vehicle_sim PRIVATE TireModel)
target_link_libraries(vehicle_sim PRIVATE Utils)
target_link_libraries(vehicle_sim PRIVATE Integrator)
target_link_libraries(vehicle_sim PRIVATE Test)

