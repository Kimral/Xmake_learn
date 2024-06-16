target("lib_imgui")
    set_kind("static")

    add_deps("lib_vulkan", "lib_SDL2", {public = true})

    add_includedirs("src/", "src/backends/", {public = true})

    add_files("src/*.cpp")
    add_files("src/backends/imgui_impl_sdl2.cpp")
    add_files("src/backends/imgui_impl_vulkan.cpp");

target_end()