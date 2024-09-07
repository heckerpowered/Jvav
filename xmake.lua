add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate")

function main(package, opt)
    if opt.system then
        local llvm_config = "llvm-config"
        if package:is_plat("macosx") then
            local llvm = try {function () return os.iorunv("brew", {"--prefix", "llvm"}) end}
            if llvm then
                local ret = find_tool("llvm-config", {paths = path.join(llvm:trim(), "bin")})
                if ret then
                    llvm_config = ret.program
                end
            end
        end
        local version = try {function() return os.iorunv(llvm_config, {"--version"}) end}
        if version then
            version = version:trim()
        end
        if package:is_toolchain() then
            if version then
                return {version = version}
            end
        else
            local linkdir = try {function() return os.iorunv(llvm_config, {"--libdir"}) end}
            local includedir = try {function() return os.iorunv(llvm_config, {"--includedir"}) end}
            if linkdir and includedir then
                linkdir = linkdir:trim()
                includedir = includedir:trim()
                local result = {version = version, links = {}, linkdirs = linkdir, includedirs = includedir}
                local components = {}
                for _, file in ipairs(os.files(path.join(os.scriptdir(), "components", "*.lua"))) do
                    local name = path.basename(file)
                    local links = {}
                    for _, link in ipairs(import("components." .. name).get_links(package)) do
                        local filename_static = target.filename(link, "static", {plat = package:plat(), arch = package:arch()})
                        local filename_shared = target.filename(link, "shared", {plat = package:plat(), arch = package:arch()})
                        if os.isfile(path.join(linkdir, filename_static)) or
                            os.isfile(path.join(linkdir, filename_shared)) then
                            table.insert(links, link)
                            table.insert(result.links, link)
                        end
                    end
                    if #links > 0 then
                        components[name] = components[name] or {}
                        components[name].links = links
                    end
                end
                components.__base = {linkdirs = linkdir, includedirs = includedir}
                result.components = components
                return result
            end
        end
    end
end

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

package("libllvm")
do
    on_install(function (package)
        import("lib.detect.find_tool")
        local llvm_config = find_tool("llvm-config")

        local cflags_raw    = os.iorunv(llvm_config.program, { "--cflags" })
        local cppflags_raw  = os.iorunv(llvm_config.program, { "--cppflags" })
        local cxxflags_raw  = os.iorunv(llvm_config.program, { "--cxxflags" })
        local ldflags_raw   = os.iorunv(llvm_config.program, { "--ldflags" })

        local cflags = cflags_raw:split("%s+") or {}
        local cppflags = cppflags_raw:split("%s+") or {}
        local cxxflags = cxxflags_raw:split("%s+") or {}
        local ldflags = ldflags_raw:split("%s+") or {}

        package:add("cflags", cflags)
        package:add("cxflags", cppflags)
        package:add("cxxflags", cxxflags)
        package:add("ldflags", ldflags)
    end)
end
package_end()

add_requires("fast_io")
add_requires("libllvm")

target("Jvav")
    set_kind("binary")
    add_files("src/Mamba/**.cpp")
    add_headerfiles("src/Mamba/**.h")
    add_packages("fast_io")
    set_toolchains("llvm")
    set_filename("mamba")
    add_languages("clatest", "c++latest")
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

    -- add_includedirs("/opt/homebrew/opt/llvm/include")
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