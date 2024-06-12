add_requires("catch2")

target("lib_catch2")
    set_kind("static")
    add_packages("catch2", {public = true})
target_end()
