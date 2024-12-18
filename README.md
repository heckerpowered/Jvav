<h1 align="center">Jvav Programming Language</h1>

Jvav is a programming language first purposed by Dr. Haoyang Zhang and implemented by Heckerpowered

### Language Standard

Visit [Working Draft](docs/draft/Jvav%2024.pdf) for language standards. To contribute, please create Issue, or make changes to the Working Draft directly, then create a Pull Request.

### Mamba

Mamba is the open-source implementation of the Jvav programming language with LLVM for building code.

### Contributing

#### Install LLVM

*Mamba requires a full LLVM to build the code. To verify that the existing LLVM is full, follow step 4 to verify*

1. Download LLVM

Download the latest LLVM release from [LLVM repository](https://github.com/llvm/llvm-project/releases/tag/llvmorg-19.1.0). (e.g. LLVM-19.1.0-macOS-ARM64.tar.xz or LLVM-19.1.0-Windows-X64.tar.xz)

2. Extract the downloaded file

Extract the file downloaded in the first step.

3. Set environment variables

Add *llvm-path*/bin to the environment variable, ensure that the toolchain can find llvm related tools.

4. Test LLVM

Run the command *llvm-config --version*, which should show the version of llvm-config.

### Clone repository

Clone https://github.com/heckerpowered/Jvav

```
git clone https://github.com/heckerpowered/Jvav.git
```

#### Developing with CMake 

Recommend use *ninja*

```
cmake -B build
cmake --build build -j$nproc
ln -s build/compile_commands.json compile_commands.json
```

#### Developing with XMake

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

#### Running Tests with CMake

To run the tests with CMake, just build and run the mamba_tests target.

```
cmake --build build -j
build/mamba_tests
```

#### Running Tests with XMake

To run the tests with XMake, just build and run the test target.

```
xmake
xmake run test
```

### Debugging with Visual Studio Code

1. Install [Visual Studio Code](https://code.visualstudio.com)
    - After install Visual Studio Code, install the [CodeLLDB](https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb) extension.
2. You can build from VS Code by running the *Run Build Task* command, then selecting an appropriate task such as *CMake: Build*.
3. You can launch debug session by running the *Start Debugging* command.
    - Build task will run automatically before starting the debug session.

## Stats

![Alt](https://repobeats.axiom.co/api/embed/9d883f8bbbda01e4daecabafda78f2a9f7971d42.svg "Repobeats analytics image")

## Website

Our website is available at [here](https://jvav.heckerpowered.net/).

<img width="1512" alt="70d80c91d18ff0554455ba09eb219e88" src="https://github.com/user-attachments/assets/81a4aac1-d18c-4085-81dd-d3a0b008446c">

