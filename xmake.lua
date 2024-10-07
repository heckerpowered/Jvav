add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")

local includedirs = {
    "Mamba/src", 
    "Mamba/src/Core", 
    "Mamba/src/Core/Printer",
    "Mamba/src/Core/International", 
    "Mamba/src/Code Analysis", 
    "Mamba/src/Code Analysis/Syntax", 
    "Mamba/src/Code Analysis/Text", 
    "Mamba/src/Code Analysis/Binding", 
    "Mamba/src/Code Analysis/Symbol",
    "Mamba/src/Code Generation",
    "Mamba/src/Code Generation/LLVM"
}

local llvm_config = "/opt/homebrew/opt/llvm/bin/llvm-config"

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
        target:add("cxxflags", cxxflags)
        target:add("ldflags", ldflags)
        target:add("ldflags", libflags_raw, {force = true})
    end)
end
rule_end()

add_requires("fast_io")
add_rules("libllvm")

target("mamba")
    set_kind("binary")
    add_files("Mamba/src/**.cpp")
    add_packages("fast_io")
    set_toolchains("llvm")
    set_filename("mamba")
    add_languages("clatest", "c++latest")
    set_warnings("all", "extra")
    add_includedirs(includedirs)
    if is_os("macosx") or is_os("linux") then
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

    -- add_includedirs("/opt/homebrew/opt/llvm/include")
    -- add_cxxflags("$(shell " .. llvm_config .. " --cxxflags)")
    -- add_ldflags("$(shell " .. llvm_config .. " --ldflags)")
    -- add_ldflags("$(shell " .. llvm_config .. " --libs all)", {force=true})
    -- add_cxxflags("-march=native")
    -- add_syslinks("$(shell " .. llvm_config .. " --system-libs)")
    
target_end()


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