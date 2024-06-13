add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

add_rules("mode.debug", "mode.release")

set_languages("c++20")

target("lib_imgui")
    set_kind("static")
    add_includedirs("src/imgui", {public = true})
    add_files("src/imgui/*.cpp")
target_end()

target("App")
    set_kind("binary")
    set_languages()
    add_deps("lib_imgui")
    add_files("src/**.cpp")
target_end()
