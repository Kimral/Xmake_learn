add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
add_rules("mode.debug", "mode.release")

add_requires("catch2 3.x")
add_requires("imgui", {configs = {glfw_opengl3 = true}})

set_plat(os.host())
set_arch(os.arch())

target("App")
    set_kind("binary")
    set_version("0.0.1")

    add_packages("imgui")
    
    add_files("src/*.cpp")
target_end()

target("Tests")
    set_kind("binary")

    add_packages("catch2")

    add_packages("imgui")

    remove_files("src/main.cpp")

    add_files("src/*.cpp")
    add_files("tests/*.cpp")
target_end()