target("App")
    set_kind("binary")
    set_languages("c++20", "c90")
    set_optimize("fastest")
    add_deps("lib_imgui")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")

    for _, dir in ipairs(os.dirs("src/**")) do
        print(dir)
        add_includedirs(dir)
    end

target_end()
