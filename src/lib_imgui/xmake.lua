target("lib_imgui")
    set_kind("static")

    add_deps("lib_SDL2", {public = true})

    add_headerfiles("src/**.h")
    add_includedirs("imgui/", "imgui/backends/", {public = true})

    add_files("imgui/*.cpp")
    add_files("imgui/backends/imgui_impl_sdl2.cpp")
    add_files("imgui/backends/imgui_impl_opengl3.cpp");

    add_links("opengl32", {public = true})

target_end()