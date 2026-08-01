cmake_minimum_required(VERSION 3.20)

project(CPP-Quantum-Math-Libraries
    VERSION 1.0.0
    DESCRIPTION "High-performance C++20 Quantum Computing and Statevector Simulation Library"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)

if(MSVC)
    add_compile_options(/openmp:llvm)
else()
    add_compile_options(${OpenMP_CXX_FLAGS})
endif()


add_executable(qml_benchmark benchmark.cpp)

target_include_directories(qml_benchmark PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(qml_benchmark PRIVATE
        -O3
        -march=native
        -ffast-math
        -flto
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
    )
    target_link_options(qml_benchmark PRIVATE -flto)
elseif(MSVC)
    target_compile_options(qml_benchmark PRIVATE
        /O2
        /arch:AVX2
        /fp:fast
        /GL
        /W4
    )
    target_link_options(qml_benchmark PRIVATE /LTCG)
endif()

target_link_libraries(qml_benchmark PRIVATE 
    OpenMP::OpenMP_CXX
)
