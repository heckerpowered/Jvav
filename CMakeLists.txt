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

include_directories("include" "Mamba/src" "Mamba/src/Core" "Mamba/src/Core/Printer" "Mamba/src/Core/International" "Mamba/src/Code Analysis" "Mamba/src/Code Analysis/Syntax" "Mamba/src/Code Analysis/Text" "Mamba/src/Code Analysis/Binding" "Mamba/src/Code Analysis/Symbol" "Mamba/src/Code Generation" "Mamba/src/Code Generation/LLVM")

# set(CMAKE_EXE_LINKER_FLAGS "-static")

file(GLOB_RECURSE srcs "Mamba/src/**.cpp")
add_executable(mamba ${srcs})
target_link_libraries(mamba PRIVATE FAST_IO)

# LLVM Embedding
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# llvm_map_components_to_libnames(llvm_libs AArch64 AArch64asmparser AArch64codegen AArch64desc AArch64disassembler AArch64info AArch64utils aggressiveinstcombine all all-targets amdgpu amdgpuasmparser amdgpucodegen amdgpudesc amdgpudisassembler amdgpuinfo amdgputargetmca amdgpuutils analysis arm armasmparser armcodegen armdesc armdisassembler arminfo armutils asmparser asmprinter avr avrasmparser avrcodegen avrdesc avrdisassembler avrinfo binaryformat bitreader bitstreamreader bitwriter bpf bpfasmparser bpfcodegen bpfdesc bpfdisassembler bpfinfo cfguard codegen codegendata codegentypes core coroutines coverage debuginfobtf debuginfocodeview debuginfodwarf debuginfogsym debuginfologicalview debuginfomsf debuginfopdb demangle dlltooldriver dwarflinker dwarflinkerclassic dwarflinkerparallel dwp engine executionengine extensions filecheck frontenddriver frontendhlsl frontendoffloading frontendopenacc frontendopenmp fuzzercli fuzzmutate globalisel hexagon hexagonasmparser hexagoncodegen hexagondesc hexagondisassembler hexagoninfo hipstdpar instcombine instrumentation interfacestub interpreter ipo irprinter irreader jitlink lanai lanaiasmparser lanaicodegen lanaidesc lanaidisassembler lanaiinfo libdriver lineeditor linker loongarch loongarchasmparser loongarchcodegen loongarchdesc loongarchdisassembler loongarchinfo lto mc mca mcdisassembler mcjit mcparser mips mipsasmparser mipscodegen mipsdesc mipsdisassembler mipsinfo mirparser msp430 msp430asmparser msp430codegen msp430desc msp430disassembler msp430info native nativecodegen nvptx nvptxcodegen nvptxdesc nvptxinfo objcarcopts objcopy object objectyaml option orcdebugging orcjit orcshared orctargetprocess passes powerpc powerpcasmparser powerpccodegen powerpcdesc powerpcdisassembler powerpcinfo profiledata remarks riscv riscvasmparser riscvcodegen riscvdesc riscvdisassembler riscvinfo riscvtargetmca runtimedyld sandboxir scalaropts selectiondag sparc sparcasmparser sparccodegen sparcdesc sparcdisassembler sparcinfo support symbolize systemz systemzasmparser systemzcodegen systemzdesc systemzdisassembler systemzinfo tablegen target targetparser textapi textapibinaryreader transformutils ve veasmparser vecodegen vectorize vedesc vedisassembler veinfo webassembly webassemblyasmparser webassemblycodegen webassemblydesc webassemblydisassembler webassemblyinfo webassemblyutils windowsdriver windowsmanifest x86 x86asmparser x86codegen x86desc x86disassembler x86info x86targetmca xcore xcorecodegen xcoredesc xcoredisassembler xcoreinfo xray)

# message(STATUS $"${llvm_libs}")

target_link_libraries(mamba PRIVATE LLVM)# ${llvm_libs})

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})