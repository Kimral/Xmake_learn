target("App")
    set_kind("binary")
    set_languages()
    add_deps("lib_SDL2", "lib_imgui")
    add_files("src/*.cpp")
target_end()
