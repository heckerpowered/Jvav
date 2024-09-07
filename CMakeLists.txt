cmake_minimum_required(VERSION 3.10)

project(Mamba CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(MAGIC_CPP_INCLUDE_TESTS OFF)

file(GLOB SOURCE "src/Mamba/**.cpp")
add_executable(Mamba ${SOURCE})

include(FetchContent)

FetchContent_Declare(
    fast_io
    URL https://github.com/cppfastio/fast_io
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

FetchContent_MakeAvailable(fast_io)