#!/bin/bash

# Default values
BUILD_TYPE="Debug"
CLEAN_BUILD=0
INTEGRATOR="Euler"
MODEL="Bicycle"

# Parse command-line arguments
while [[ "$#" -gt 0 ]]; do
    case $1 in
        --int) INTEGRATOR="$2"; shift ;;
        --model) MODEL="$2"; shift ;;
        -d) BUILD_TYPE="Debug" ;;
        -r) BUILD_TYPE="Release" ;;
        -m) CLEAN_BUILD=1 ;;
        -h|--help)
            echo "Usage: ./buildit [options]"
            echo "Options:"
            echo "  --int <integrator>   Specify the integrator to build (e.g., Euler, RK4)"
            echo "  --model <model>      Specify the model to build (e.g., Bicycle, HighFidelity)"
            echo "  -d                   Build in Debug mode (default)"
            echo "  -r                   Build in Release mode"
            echo "  -m                   Perform a clean build"
            echo "  -h, --help           Show this help message"
            exit 0
            ;;
        *) echo "Unknown option: $1"; exit 1 ;;
    esac
    shift
done

# Ensure integrator and model are provided
if [[ -z "$INTEGRATOR" || -z "$MODEL" ]]; then
    echo "Error: Both --int and --model must be specified."
    echo "Use './buildit -h' for help."
    exit 1
fi

model=${MODEL^^}
integrator=${INTEGRATOR^^}

# Define build directory
BUILD_DIR="build"

# Clean build if requested
if [[ $CLEAN_BUILD -eq 1 ]]; then
    echo "Performing clean build..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Run CMake with correct flags
echo "Running CMake with -DUSE_${model}_MODEL=ON -DUSE_${integrator}_INTEGRATION=ON..."
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DUSE_${MODEL^^}_MODEL=ON -DUSE_${INTEGRATOR^^}_INTEGRATION=ON

# Build project
echo "Building project..."
cmake --build . --config $BUILD_TYPE

# Copy compile_commands.json to root directory
if [[ -f "compile_commands.json" ]]; then
    cp compile_commands.json ..
fi

