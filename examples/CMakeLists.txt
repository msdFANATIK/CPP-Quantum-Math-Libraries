cmake_minimum_required(VERSION 3.14)
project(qm_examples LANGUAGES CXX)

# Enforce C++20 standard globally for examples
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add executable targets
add_executable(hello_qm hello_qm.cpp)
add_executable(vector_demo vector_demo.cpp)

# Target include directory resolves directly from project root
target_include_directories(hello_qm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(vector_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Optional: Add compiler flags for zero-overhead optimization checks
if(MSVC)
    target_compile_options(hello_qm PRIVATE /W4 /O2)
    target_compile_options(vector_demo PRIVATE /W4 /O2)
else()
    target_compile_options(hello_qm PRIVATE -Wall -Wextra -O3)
    target_compile_options(vector_demo PRIVATE -Wall -Wextra -O3)
endif()
