cmake_minimum_required(VERSION 3.10)
project(HPC_DISEASE_SIMULATION_SREEDEVI)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Enable code coverage
option(ENABLE_COVERAGE "Enable code coverage" ON)

if(ENABLE_COVERAGE AND CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

# Include directories
include_directories(include)

# Source files for the main executable
set(SOURCES
    simulation/simulation.cpp  # Main simulation logic
    simulation/main.cpp        # Entry point for the simulation
)

# Source files for the tests
set(TEST_SOURCES
    simulation/simulation.cpp  # Reuses simulation logic
    simulation/test.cpp        # Test cases for the simulation
)

# Main executable
add_executable(disease_simulation ${SOURCES})

# Test executable
add_executable(disease_tests ${TEST_SOURCES})

# Link the standard C++ library explicitly (if needed for filesystem or threading)
target_link_libraries(disease_simulation PRIVATE stdc++)
target_link_libraries(disease_tests PRIVATE stdc++)

# Enable verbose makefile for debugging
set(CMAKE_VERBOSE_MAKEFILE ON)

# Enable testing
enable_testing()

# Register the test executable with CTest
add_test(NAME DiseaseSimulationTests COMMAND disease_tests)

# Copy disease_in.ini to the build directory (for test purpose)
#configure_file(${CMAKE_SOURCE_DIR}/disease_in.ini ${CMAKE_BINARY_DIR}/disease_in.ini COPYONLY)
