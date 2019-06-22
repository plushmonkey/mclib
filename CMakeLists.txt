cmake_minimum_required(VERSION 3.1)
project(mclib)

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup()

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(mclib)
add_subdirectory(client)
add_subdirectory(tests)
