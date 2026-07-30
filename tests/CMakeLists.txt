cmake_minimum_required(VERSION 3.20)
project(QuantumPP_MiniEngineTEST VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler Optimization Flags
if(MSVC)
    add_compile_options(/O2 /W4)
else()
    # Omit -march=native to guarantee binary compatibility across GitHub Actions runners
    add_compile_options(-O3 -Wall -Wextra -pedantic)
endif()

# Core Include Directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

# Core Integration Test Executable
add_executable(qpp_test 1.cpp)

# Enable CTest Target Execution
enable_testing()
add_test(NAME IntegrationTestSuite COMMAND qpp_test)
