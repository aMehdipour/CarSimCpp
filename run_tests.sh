#!/bin/bash

# Colors for output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "Running CarSimCpp Unit Tests"
echo "============================"

# Check if build directory exists
if [ ! -d "build" ]; then
    echo -e "${RED}Error: build directory not found. Run ./buildit.sh first.${NC}"
    exit 1
fi

# Check if tests directory exists
if [ ! -d "build/tests" ]; then
    echo -e "${RED}Error: build/tests directory not found. Tests may not have been built.${NC}"
    exit 1
fi

# Run tire model tests
echo -e "\n${GREEN}Running Tire Model Tests...${NC}"
if [ -f "build/tests/test_tire_model" ]; then
    ./build/tests/test_tire_model
    TIRE_TEST_RESULT=$?
else
    echo -e "${RED}test_tire_model not found${NC}"
    TIRE_TEST_RESULT=1
fi

# Run integrator tests
echo -e "\n${GREEN}Running Integrator Tests...${NC}"
if [ -f "build/tests/test_integrator" ]; then
    ./build/tests/test_integrator
    INTEGRATOR_TEST_RESULT=$?
else
    echo -e "${RED}test_integrator not found${NC}"
    INTEGRATOR_TEST_RESULT=1
fi

# Summary
echo -e "\n${GREEN}Test Summary:${NC}"
echo "============================="

if [ $TIRE_TEST_RESULT -eq 0 ]; then
    echo -e "Tire Model Tests: ${GREEN}PASSED${NC}"
else
    echo -e "Tire Model Tests: ${RED}FAILED${NC}"
fi

if [ $INTEGRATOR_TEST_RESULT -eq 0 ]; then
    echo -e "Integrator Tests: ${GREEN}PASSED${NC}"
else
    echo -e "Integrator Tests: ${RED}FAILED${NC}"
fi

# Exit with error if any test failed
if [ $TIRE_TEST_RESULT -ne 0 ] || [ $INTEGRATOR_TEST_RESULT -ne 0 ]; then
    exit 1
fi

echo -e "\n${GREEN}All tests passed!${NC}"
exit 0