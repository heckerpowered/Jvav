cmake_minimum_required(VERSION 3.22)
project(jvav)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT MSVC)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

include(FetchContent)
FetchContent_Declare(
        fastio
        URL https://github.com/Adttil/fast_io/archive/refs/heads/master.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(fastio)

include_directories("include" "src/Mamba" "src/Mamba/Core" "src/Mamba/Code Analysis" "src/Mamba/Code Analysis/Syntax" "src/Mamba/Code Analysis/Text" "src/Mamba/Code Analysis/Binding" "src/Mamba/Code Analysis/Symbol" "src/Mamba/Code Analysis/Emit" "src/Mamba/Code Analysis/Linking")

set(CMAKE_EXE_LINKER_FLAGS "-static")

file(GLOB_RECURSE srcs "src/Mamba/**.cpp")
add_executable(Mamba ${srcs})
target_link_libraries(Mamba PRIVATE FAST_IO)