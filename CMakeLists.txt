cmake_minimum_required(VERSION 3.16)



project(SecsGemBridge LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/short_path.cmake)
include(cmake/platform.cmake)
add_subdirectory(src)

