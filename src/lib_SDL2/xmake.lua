target("lib_SDL2")
    set_kind("static")
    add_includedirs("src/include/", {public = true})
    add_headerfiles("src/include/*.h")
    add_linkdirs("src/lib/x64/", {public = true})

    add_ldflags("/SUBSYSTEM:CONSOLE", {public = true})
target_end()