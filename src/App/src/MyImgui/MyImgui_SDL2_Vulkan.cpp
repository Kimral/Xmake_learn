#include "MyImgui_SDL2_Vulkan.h"

MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::MyImgui() {
    // Create Window Surface
    VkSurfaceKHR surface;
    if (SDL_Vulkan_CreateSurface(m_InputHandler.GetWindow(), m_InputHandler.GetRender().GetInstance(), &surface) == 0)
    {
        throw std::runtime_error(SDL_GetError());
    }

    // Create Framebuffers
    int w, h;
    SDL_GetWindowSize(m_InputHandler.GetWindow(), &w, &h);
    SetupVulkanWindow(surface, w, h);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForVulkan(m_InputHandler.GetWindow());
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = m_InputHandler.GetRender().GetInstance();
    init_info.PhysicalDevice = m_InputHandler.GetRender().GetPhysicalDevice();
    init_info.Device = m_InputHandler.GetRender().GetDevice();
    init_info.QueueFamily = m_InputHandler.GetRender().GetQueueFamily();
    init_info.Queue = m_InputHandler.GetRender().GetQueue();
    init_info.PipelineCache = m_InputHandler.GetRender().GetPipelineCache();
    init_info.DescriptorPool = m_InputHandler.GetRender().GetDescriptorPool();
    init_info.RenderPass = m_MainWindowData.RenderPass;
    init_info.Subpass = 0;
    init_info.MinImageCount = 2;// g_MinImageCount
    init_info.ImageCount = m_MainWindowData.ImageCount;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = m_InputHandler.GetRender().GetAllocator();
    init_info.CheckVkResultFn = m_InputHandler.GetRender().GetCheckVkResultFunction();;
    ImGui_ImplVulkan_Init(&init_info);
}

MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::~MyImgui() {
    // Cleanup
    VkResult err = vkDeviceWaitIdle(m_InputHandler.GetRender().GetDevice());
    m_InputHandler.GetRender().check_vk_result(err);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    ImGui_ImplVulkanH_DestroyWindow(m_InputHandler.GetRender().GetInstance(), 
                                    m_InputHandler.GetRender().GetDevice(),
                                    &m_MainWindowData,
                                    m_InputHandler.GetRender().GetAllocator());
    
}

void MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::CleanupVulkanWindow()
{
    ImGui_ImplVulkanH_DestroyWindow(m_InputHandler.GetRender().GetInstance(), 
                                    m_InputHandler.GetRender().GetDevice(),
                                    &m_MainWindowData, 
                                    m_InputHandler.GetRender().GetAllocator());
}

void MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::Run(std::function<void()> to_Do) {
    // Main loop
    while (!m_done)
    {
        ProcessEvent();
        NewFrame();
        to_Do();
        ResizeSwapChain();
        FinishFrame();
    }
}

void MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::SetClearColor(float r, float g, float b, float a) {
    m_clear_color = ImVec4{ r, g, b, a };
}

ImVec4& MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::GetClearColor() {
    return m_clear_color;
}

ImGuiIO& MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::GetIO() {
    return ImGui::GetIO();
}

void MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::NewFrame() {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::ProcessEvent() {
    while (m_InputHandler.PollEvent())
    {
        const SDL_Event& l_Event = m_InputHandler.GetEvent();
        ImGui_ImplSDL2_ProcessEvent(&l_Event);
        if (l_Event.type == SDL_QUIT) {
            m_done = true;
        } 
        if (l_Event.type == SDL_WINDOWEVENT &&
            l_Event.window.event == SDL_WINDOWEVENT_CLOSE &&
            l_Event.window.windowID == m_InputHandler.GetWindowId(m_InputHandler.GetWindow())) {
            m_done = true;
        }
    }
}

void MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::FinishFrame() {
    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
    if (!is_minimized)
    {
        m_MainWindowData.ClearValue.color.float32[0] = m_clear_color.x * m_clear_color.w;
        m_MainWindowData.ClearValue.color.float32[1] = m_clear_color.y * m_clear_color.w;
        m_MainWindowData.ClearValue.color.float32[2] = m_clear_color.z * m_clear_color.w;
        m_MainWindowData.ClearValue.color.float32[3] = m_clear_color.w;

        
        FrameRender(draw_data);
        FramePresent();
    }
}

void MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::ResizeSwapChain() {
    // Resize swap chain?
    int fb_width, fb_height;
    SDL_GetWindowSize(m_InputHandler.GetWindow(), &fb_width, &fb_height);
    if (fb_width > 0 && 
        fb_height > 0 && 
        (m_SwapChainRebuild || m_MainWindowData.Width != fb_width || m_MainWindowData.Height != fb_height))
    {
        ImGui_ImplVulkan_SetMinImageCount(m_MinImageCount);
        ImGui_ImplVulkanH_CreateOrResizeWindow(m_InputHandler.GetRender().GetInstance(),
                                               m_InputHandler.GetRender().GetPhysicalDevice(),
                                               m_InputHandler.GetRender().GetDevice(),
                                               &m_MainWindowData, 
                                               m_InputHandler.GetRender().GetQueueFamily(),
                                               m_InputHandler.GetRender().GetAllocator(),
                                               fb_width, 
                                               fb_height, 
                                               m_MinImageCount);
        m_MainWindowData.FrameIndex = 0;
        m_SwapChainRebuild = false;
    }
}

void MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::FrameRender(ImDrawData* draw_data)
{
    VkResult err;

    ImGui_ImplVulkanH_FrameSemaphores* FrameSemaphor = m_MainWindowData.FrameSemaphores;
    VkSemaphore image_acquired_semaphore = FrameSemaphor[m_MainWindowData.SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore render_complete_semaphore = FrameSemaphor[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore;
    err = vkAcquireNextImageKHR(m_InputHandler.GetRender().GetDevice(), 
                                m_MainWindowData.Swapchain, 
                                UINT64_MAX, 
                                image_acquired_semaphore, 
                                VK_NULL_HANDLE,
                                &m_MainWindowData.FrameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        m_SwapChainRebuild = true;
        return;
    }
    m_InputHandler.GetRender().check_vk_result(err);

    auto device = m_InputHandler.GetRender().GetDevice();
    ImGui_ImplVulkanH_Frame* fd = &m_MainWindowData.Frames[m_MainWindowData.FrameIndex];
    {
        err = vkWaitForFences(device, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        m_InputHandler.GetRender().check_vk_result(err);

        err = vkResetFences(device, 1, &fd->Fence);
        m_InputHandler.GetRender().check_vk_result(err);
    }
    {
        err = vkResetCommandPool(device, fd->CommandPool, 0);
        m_InputHandler.GetRender().check_vk_result(err);
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
        m_InputHandler.GetRender().check_vk_result(err);
    }
    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = m_MainWindowData.RenderPass;
        info.framebuffer = fd->Framebuffer;
        info.renderArea.extent.width = m_MainWindowData.Width;
        info.renderArea.extent.height = m_MainWindowData.Height;
        info.clearValueCount = 1;
        info.pClearValues = &m_MainWindowData.ClearValue;
        vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

    // Submit command buffer
    vkCmdEndRenderPass(fd->CommandBuffer);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &image_acquired_semaphore;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &fd->CommandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &render_complete_semaphore;

        err = vkEndCommandBuffer(fd->CommandBuffer);
        m_InputHandler.GetRender().check_vk_result(err);
        err = vkQueueSubmit(m_InputHandler.GetRender().GetQueue(), 1, &info, fd->Fence);
        m_InputHandler.GetRender().check_vk_result(err);
    }
}

void MyImgui<SDL2_InHandler<Vulkan_Render>, Vulkan_Render>::FramePresent()
{
    if (m_SwapChainRebuild)
        return;
    VkSemaphore render_complete_semaphore = m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore;
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &render_complete_semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &m_MainWindowData.Swapchain;
    info.pImageIndices = &m_MainWindowData.FrameIndex;
    VkResult err = vkQueuePresentKHR(m_InputHandler.GetRender().GetQueue(), &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        m_SwapChainRebuild = true;
        return;
    }
    m_InputHandler.GetRender().check_vk_result(err);
    m_MainWindowData.SemaphoreIndex = (m_MainWindowData.SemaphoreIndex + 1) % m_MainWindowData.SemaphoreCount; // Now we can use the next set of semaphores
}
