target("lib_vulkan")
    set_kind("static")
    add_includedirs("D:/Apps/vulkan_SDK/Include/", {public = true})

    add_linkdirs("D:/Apps/vulkan_SDK/Lib/", {public = true})
    add_links("vulkan-1", {public = true})
target_end()