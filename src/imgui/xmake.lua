target("lib_imgui")
    set_kind("static")
    add_includedirs("src/", {public = true})
    add_files("src/*.cpp")
target_end()