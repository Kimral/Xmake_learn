target("lib_vulkan")
    set_kind("static")

    add_headerfiles("D:/Apps/vulkan_SDK/Include/**.h")
    add_links("vulkan-1", {public = true})
    
    add_includedirs("D:/Apps/vulkan_SDK/Include/", {public = true})
    add_linkdirs("D:/Apps/vulkan_SDK/Lib/", {public = true})
target_end()