add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")


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
add_requires("fast_io")
set_runtimes("c++_shared")

target("Jvav")
    add_languages("c++23")
    add_cxxflags("-std=c++23")
    set_kind("binary")
    add_files("src/Mamba/**.cpp")
    add_headerfiles("src/Mamba/**.h")
    add_packages("fast_io")
    set_toolchains("llvm")
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

    add_includedirs("/opt/homebrew/opt/llvm/include")
    -- add_cxxflags("$(shell " .. llvm_config .. " --cxxflags)")
    add_ldflags("$(shell " .. llvm_config .. " --ldflags)")
    add_ldflags("$(shell " .. llvm_config .. " --libs all)", {force=true})
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