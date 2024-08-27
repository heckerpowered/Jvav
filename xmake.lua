add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")

add_requires("fast_io")

local includedirs = {
    "src/Mamba", 
    "src/Mamba/Core", 
    "src/Mamba/Code Analysis", 
    "src/Mamba/Code Analysis/Syntax", 
    "src/Mamba/Code Analysis/Text", 
    "src/Mamba/Code Analysis/Binding", 
    "src/Mamba/Code Analysis/Symbol",
    "src/Mamba/Code Analysis/Linking",
    "src/Mamba/Code Generation",
}

local llvm_config = "/opt/homebrew/opt/llvm/bin/llvm-config"


set_languages("c++20")
add_languages("c++26")

target("Jvav")
    set_kind("binary")
    add_files("src/Mamba/**.cpp")
    add_headerfiles("src/Mamba/**.h")
    add_packages("fast_io")
    set_toolchains("clang")
    set_filename("mamba")
    set_warnings("all", "extra")
    add_includedirs(includedirs)
    if is_os("macosx") then
        add_linkdirs("/opt/homebrew/opt/llvm/lib/c++") -- macOS compability
        add_includedirs("/opt/homebrew/opt/llvm/include")
    end
    if is_mode("release") then
        set_optimize("fastest")
    end
    if is_mode("debug") then
        -- add_cflags("-fsanitize=address")
        -- add_ldflags("-fsanitize=address", "-fsanitize=undefined")
        -- add_cxxflags("-fsanitize=address", "-fno-omit-frame-pointer", "-fno-optimize-sibling-calls")
        add_defines("DEBUG")
    end

    add_cxxflags("$(shell " .. llvm_config .. " --cxxflags)")
    add_ldflags("$(shell " .. llvm_config .. " --ldflags)")
    add_syslinks("$(shell " .. llvm_config .. " --system-libs)")


for _, file in ipairs(os.files("src/Test/**.cpp")) do
    local name = path.basename(file)
    target(name)
        set_kind("binary")
        set_default(false)
        add_files("src/Mamba/**.cpp")
        add_headerfiles("src/Mamba/**.h")
        set_languages("clatest", "c++latest")
        add_includedirs(includedirs)
        add_files("src/Test/" .. name .. ".cpp")
        add_packages("fast_io")
        add_tests("default")
end

--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

