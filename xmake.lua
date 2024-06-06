add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})
add_rules("mode.debug", "mode.release")

add_requires("catch2 3.x")
add_requires("imgui", {configs = {glfw_opengl3 = true}})

set_plat(os.host())
set_arch(os.arch())

target("__Inner_targer")
    set_kind("static")

    set_default(false)

    add_packages("imgui", {public = true})
    --add_files("src/**.cpp")
    add_headerfiles("src/**.h")
target_end()


target("App")
    set_kind("binary")
    set_version("0.0.1")

    add_deps("__Inner_targer")

    add_files("main.cpp")

target_end()


target("Tests")
    set_kind("binary")

    add_packages("catch2")

    add_deps("__Inner_targer")

    add_files("tests/**.cpp")
target_end()