## Introduction

Jvav is a programming language first purposed by Dr. Haoyang Zhang and implemented by [Heckerpowered](https://github.com/heckerpowered)

## Language Standard

[Working draft for Jvav](docs/draft/Jvav%2024.pdf)

## Mamba

Mamba is the compiler of the Jvav programming language, it can compile Jvav code into source code of other programming languages such as C++, C#, Java and more.

## Contributing

### Install LLVM

*If you have installed LLVM via package manager (e.g. homebrew or scoop), make sure there's llvm-config in \<llvm-path\>/bin/ directory, then skip this step.*

1. Download LLVM

Download the latest LLVM release from [LLVM repository](https://github.com/llvm/llvm-project/releases/tag/llvmorg-19.1.0). (e.g. LLVM-19.1.0-macOS-ARM64.tar.xz or LLVM-19.1.0-Windows-X64.tar.xz)

2. Extract the downloaded file

Extract the downloaded file to a directory of your choice, we called it *installation path*.

3. Set environment variables

Add the bin directory of the LLVM installation path to your PATH environment variable.

4. Test LLVM

Open a terminal and run the following command:

Windows:

```
where.exe llvm-config
```

macOS/Linux:

```
which llvm-config
```

The which/where command should output the path to the llvm-config executable

### Clone repository

```
git clone https://github.com/heckerpowered/Jvav.git
```

#### Using CMake 

```
cmake -B build
cmake --build build -j
ln -s build/compile_commands.json compile_commands.json
```

#### Using XMake

```
xmake
```

Generate IDE Project Files:

```
xmake project -k <Name>
```

Following names are supported:
- makefile
- cmakelists
- ninja
- compiler_flags
- compile_commands
- xcode
- vsxmake

### Testing

After making a contribution, run the test to make sure everything is working as expected. Remember to change the test suite to include your new feature.

#### Test via CMake

```
cmake --build build -j
build/mamba_tests
```

#### Test via XMake

```
xmake
xmake run test
```

### Debugging

We've adapted VSCode, if you are using VSCode, just press F5 to build with CMake and use LLDB to debug.

You can also use XMake for debugging, ou only need to install the XMake plug-in in VSCode and click the debug button below.

## Stats

![Alt](https://repobeats.axiom.co/api/embed/9d883f8bbbda01e4daecabafda78f2a9f7971d42.svg "Repobeats analytics image")

