# PongAI/CMakeLists.txt

cmake_minimum_required(VERSION 3.8)
project(PongAI)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(vendors)

target_compile_features(PongAI PRIVATE cxx_std_20)
target_include_directories(PongAI PUBLIC include)