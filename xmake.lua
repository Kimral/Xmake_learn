add_rules("mode.debug", "mode.release")

set_languages("c++20")

target("App")
    set_kind("binary")
    set_languages()
    add_packages(lib_catch2)
    add_files("src/**.cpp")
target_end()
