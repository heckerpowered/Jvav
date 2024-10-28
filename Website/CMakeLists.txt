cmake_minimum_required(VERSION 3.22)

project(website)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BOOST_INCLUDE_LIBRARIES asio)
set(BOOST_ENABLE_CMAKE ON)

include(FetchContent)

FetchContent_Declare(
    fast_io
    URL https://github.com/Adttil/fast_io/archive/refs/heads/master.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_Declare(
  boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
)

FetchContent_MakeAvailable(FASTIO)
FetchContent_MakeAvailable(boost)

file(GLOB_RECURSE srcs "src/**.cpp")

add_executable(website ${srcs})
target_link_libraries(website PRIVATE FAST_IO Boost::asio)