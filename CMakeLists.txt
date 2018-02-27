cmake_minimum_required(VERSION 3.5)

# set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17) # sometimes the above does not work

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src)
add_subdirectory(tests)

