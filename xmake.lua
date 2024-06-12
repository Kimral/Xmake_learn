add_rules("mode.debug", "mode.release")

set_languages("c++20")

includes("src/*") -- Пoдключить все targets

target("App")
    set_kind("binary")
    set_languages()
    add_packages(common)
    add_files("src/**.cpp")
target_end()