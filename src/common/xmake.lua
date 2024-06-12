add_rules("mode.debug", "mode.release")

target("common")
    set_kind("static")
    add_files("src/*.cpp")
target_end()
