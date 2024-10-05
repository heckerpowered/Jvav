cmake_minimum_required(VERSION 3.22)

project(mamba)

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

include_directories("include" "src/Mamba" "src/Mamba/Core" "src/Mamba/Code Analysis" "src/Mamba/Code Analysis/Syntax" "src/Mamba/Code Analysis/Text" "src/Mamba/Code Analysis/Binding" "src/Mamba/Code Analysis/Symbol" "src/Mamba/Code Generation")

set(CMAKE_EXE_LINKER_FLAGS "-static")

file(GLOB_RECURSE srcs "Mamba/src/**.cpp")
add_executable(mamba ${srcs})
target_link_libraries(mamba PRIVATE FAST_IO)

# LLVM Embedding
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(mamba PRIVATE ${llvm_libs})

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})