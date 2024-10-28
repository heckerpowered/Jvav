add_requires("boost", "fast_io")

target("website")
    set_kind("binary")
    add_files("src/**.cpp")
    add_packages("boost", "fast_io")
    set_languages("c++latest")