#pragma once

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_vulkan.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#include <SDL.h>
#include <SDL_vulkan.h>

class Application {
public:
    Application() = default;
    void Init();
    void Run();
    void Stop();
    void Shutdown();

private:
    void check_vk_result(VkResult err);
    bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
    VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
    void SetupVulkan(ImVector<const char*> instance_extensions);
    void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
    void CleanupVulkan();
    void CleanupVulkanWindow();
    void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
    void FramePresent(ImGui_ImplVulkanH_Window* wd);

    // Data
    VkAllocationCallbacks*   g_Allocator = nullptr;
    VkInstance               g_Instance = VK_NULL_HANDLE;
    VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice                 g_Device = VK_NULL_HANDLE;
    uint32_t                 g_QueueFamily = (uint32_t)-1;
    VkQueue                  g_Queue = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
    VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
    VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

    ImGui_ImplVulkanH_Window g_MainWindowData;
    uint32_t                 g_MinImageCount = 2;
    bool                     g_SwapChainRebuild = false;
    SDL_Window* window = nullptr;

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGuiIO* io = nullptr;
    VkResult err;
    ImGui_ImplVulkanH_Window* wd;
};