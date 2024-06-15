target("lib_SDL2")
    set_kind("static")
    add_includedirs("src/SDL2", {public = true})
target_end()