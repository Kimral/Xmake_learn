target("App")
    set_kind("binary")
    set_languages("c++20", "c90")
    set_optimize("fastest")
    add_deps("lib_imgui")
    add_files("src/*.cpp")

    add_includedirs("src/InputHandlers/")
    add_includedirs("src/InputHandlers/SDL2/")
    add_includedirs("src/InputHandlers/SDL2/SDL2_Opengl3/")
    
    add_includedirs("src/Renders")
target_end()
