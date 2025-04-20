# Create a library for the Integrator with selected source files
add_library(Integrator 
    PIController.cpp  # Only include the necessary .cpp file
)

# Manually list the headers (these are not compiled but included)
target_sources(Integrator PRIVATE 
    DormandPrince853.h
    Dopr853Constants.h
    IntegratorBase.h
    PIController.h
)

# Define any preprocessor macros if needed
target_compile_definitions(Integrator PUBLIC USE_DORMAND_PRINCE)

# Specify the include directories
target_include_directories(Integrator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

