cmake_minimum_required(VERSION 3.10)
project(qm_examples LANGUAGES CXX)

add_executable(hello_qm hello_qm.cpp)
target_compile_features(hello_qm PRIVATE cxx_std_20)
target_include_directories(hello_qm PRIVATE ${CMAKE_SOURCE_DIR}/../include)
