add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")

local includedirs = {
    "../fast_io/include",
    "src", 
    "src/Core", 
    "src/Core/Printer",
    "src/Core/International", 
    "src/Code Analysis", 
    "src/Code Analysis/Syntax", 
    "src/Code Analysis/Text", 
    "src/Code Analysis/Binding",
    "src/Code Analysis/Binding/Control Flow",
    "src/Code Analysis/Symbol",
    "src/Code Generation",
    "src/Code Generation/LLVM"
}

rule("libllvm")
do
    on_config(function (target)
        import("lib.detect.find_tool")
        local llvm_config = find_tool("llvm-config")

        -- local cflags_raw    = os.iorunv(llvm_config.program, { "--cflags" })
        -- local cppflags_raw  = os.iorunv(llvm_config.program, { "--cppflags" })
        -- local cxxflags_raw  = os.iorunv(llvm_config.program, { "--cxxflags" })
        local ldflags_raw   = os.iorunv(llvm_config.program, { "--ldflags" })
        local libflags_raw   = os.iorunv(llvm_config.program, { "--libs" })

        -- local cflags = cflags_raw:split("%s+") or {}
        -- local cppflags = cppflags_raw:split("%s+") or {}
        -- local cxxflags = cxxflags_raw:split("%s+") or {}
        local ldflags = ldflags_raw:split("%s+") or {}
        local libflags = libflags_raw:split("%s+") or {}

        -- target:add("cflags", cflags)
        -- target:add("cxflags", cppflags)
        target:add("cxxflags", cxxflags, { force = true })
        target:add("ldflags", ldflags, { force = true })
        target:add("ldflags", libflags_raw, {force = true})
    end)
end
rule_end()

add_rules("libllvm")
add_requires("gtest")

target("mamba")
    set_kind("binary")
    add_files("src/**.cpp")
    add_packages("fast_io")
    set_toolchains("llvm")
    set_filename("mamba")
    add_languages("clatest", "c++latest")
    set_warnings("all", "extra")
    add_includedirs(includedirs)
    set_targetdir("../build/mamba/")
    if is_os("macosx") or is_os("linux") then
        add_linkdirs("/opt/homebrew/opt/llvm/lib/c++") -- macOS compability
        add_includedirs("/opt/homebrew/opt/llvm/include")
    end
    if is_mode("release") then
        set_optimize("fastest")
    end
    if is_mode("debug") then
        -- add_cflags("-fsanitize=address")
        -- add_ldflags("-fsanitize=address")
        -- add_cxxflags("-fsanitize=address", "-fno-omit-frame-pointer", "-fno-optimize-sibling-calls")
        add_defines("DEBUG")
    end

    -- add_includedirs("/opt/homebrew/opt/llvm/include")
    -- add_cxxflags("$(shell " .. llvm_config .. " --cxxflags)")
    -- add_ldflags("$(shell " .. llvm_config .. " --ldflags)")
    -- add_ldflags("$(shell " .. llvm_config .. " --libs all)", {force=true})
    -- add_cxxflags("-march=native")
    -- add_syslinks("$(shell " .. llvm_config .. " --system-libs)")
    
target_end()

target("test")
    add_files("test/**.cpp", "src/**.cpp")
    add_packages("gtest", "fast_io", "libllvm")
    add_rules("libllvm")
    set_toolchains("llvm")
    set_warnings("all", "extra")
    set_languages("c++latest")
    add_includedirs(includedirs)
    set_targetdir("../build/mamba/")
    if is_os("macosx") or is_os("linux") then
        add_linkdirs("/opt/homebrew/opt/llvm/lib/c++")
        add_includedirs("/opt/homebrew/opt/llvm/include")
    end
    add_defines("MAMBA_TEST")
target_end()