target("lib_imgui")
    set_kind("static")

    add_deps("lib_vulkan", "lib_SDL2", {public = true})

    add_includedirs("imgui/", "imgui/backends/", {public = true})

    add_files("imgui/*.cpp")
    add_files("imgui/backends/imgui_impl_sdl2.cpp")
    add_files("imgui/backends/imgui_impl_vulkan.cpp");

target_end()