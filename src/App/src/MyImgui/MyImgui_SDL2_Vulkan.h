#pragma once

#include "imgui_impl_sdl2.h"
#include "imgui_impl_vulkan.h"

#include "MyImgui_interface.h"

#include "SDL2_Vulkan_InHandler.h"

template<>
class MyImgui<SDL2_InHandler<Vulkan_Render>> : public Imgui_Interface {

public:
    MyImgui();
    ~MyImgui() override;

    void CleanupVulkanWindow();

    void Run(std::function<void()> to_Do) override;
    void SetClearColor(float r, float g, float b, float a) override;
    ImVec4& GetClearColor() override;

    ImGuiIO& GetIO() override;


    void SetupVulkanWindow(VkSurfaceKHR surface, int width, int height)
    {
        m_MainWindowData.Surface = surface;

        // Check for WSI support
        VkBool32 res;
        vkGetPhysicalDeviceSurfaceSupportKHR(m_InputHandler.GetRender().GetPhysicalDevice(), 
                                             m_InputHandler.GetRender().GetQueueFamily(),
                                             m_MainWindowData.Surface, 
                                             &res);
        if (res != VK_TRUE)
        {
            fprintf(stderr, "Error no WSI support on physical device 0\n");
            exit(-1);
        }

        // Select Surface Format
        const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
        const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
        m_MainWindowData.SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(m_InputHandler.GetRender().GetPhysicalDevice(),
                                                                               m_MainWindowData.Surface,
                                                                               requestSurfaceImageFormat, 
                                                                               (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), 
                                                                               requestSurfaceColorSpace);

        // Select Present Mode
        #ifdef APP_UNLIMITED_FRAME_RATE
        VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
        #else
        VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
        #endif
        m_MainWindowData.PresentMode = ImGui_ImplVulkanH_SelectPresentMode(m_InputHandler.GetRender().GetPhysicalDevice(), 
            m_MainWindowData.Surface,
            &present_modes[0], 
            IM_ARRAYSIZE(present_modes));
        //printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

        // Create SwapChain, RenderPass, Framebuffer, etc.
        IM_ASSERT(m_MinImageCount >= 2);
        auto& render = m_InputHandler.GetRender();
        ImGui_ImplVulkanH_CreateOrResizeWindow(render.GetInstance(),
            render.GetPhysicalDevice(),
            render.GetDevice(),
            &m_MainWindowData, 
            render.GetQueueFamily(),
            render.GetAllocator(),
            width, 
            height, 
            m_MinImageCount);
    }

    void SetContext();
    void SetStyle();
    void InitVulkan();

private:
    void NewFrame();
    void ProcessEvent();
    void FinishFrame();
    void ResizeSwapChain();

    void SetClearColor();

    void FrameRender(ImDrawData* draw_data);
    void FramePresent();

    bool IsMinimized();

    ImDrawData* GetDrawData();

    void WaitDeviceIdle();

    void Finish() override;

private:
    SDL2_InHandler<Vulkan_Render> m_InputHandler;

    ImVec4 m_ClearColor{};
    ImGui_ImplVulkanH_Window m_MainWindowData{};
    ImGui_ImplVulkan_InitInfo m_VulkanInitInfo{};

    uint32_t m_MinImageCount = 2;
};