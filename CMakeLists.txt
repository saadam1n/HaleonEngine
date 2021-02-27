cmake_minimum_required(VERSION 3.7)
cmake_policy(SET CMP0048 NEW)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(HaleonEngine VERSION 1.0.0.0)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

add_subdirectory(lib)
add_subdirectory(Haleon)
add_subdirectory(Demo)