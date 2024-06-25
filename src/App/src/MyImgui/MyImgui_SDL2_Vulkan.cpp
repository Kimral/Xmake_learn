#include "MyImgui_SDL2_Vulkan.h"

MyImgui<SDL2_InHandler<Vulkan_Render>>::MyImgui() {
    std::pair<int, int> sizes = m_InputHandler.GetWindowSize();
    SetupVulkanWindow(m_InputHandler.GetRender().GetSurface(), sizes.first, sizes.second);;

    SetContext();
    SetStyle();
    InitVulkan();
}

MyImgui<SDL2_InHandler<Vulkan_Render>>::~MyImgui() {
    // Cleanup
    WaitDeviceIdle();
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    ImGui_ImplVulkanH_DestroyWindow(m_InputHandler.GetRender().GetInstance(), 
                                    m_InputHandler.GetRender().GetDevice(),
                                    &m_MainWindowData,
                                    m_InputHandler.GetRender().GetAllocator());
    
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::CleanupVulkanWindow()
{
    ImGui_ImplVulkanH_DestroyWindow(m_InputHandler.GetRender().GetInstance(), 
                                    m_InputHandler.GetRender().GetDevice(),
                                    &m_MainWindowData, 
                                    m_InputHandler.GetRender().GetAllocator());
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::Run(std::function<void()> to_Do) {
    while (!Done()) {
        ProcessEvent();
        ResizeSwapChain();
        NewFrame();
        to_Do();
        FinishFrame();
    }
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::SetClearColor(float r, float g, float b, float a) {
    m_ClearColor = ImVec4{ r, g, b, a };
}

ImVec4& MyImgui<SDL2_InHandler<Vulkan_Render>>::GetClearColor() {
    return m_ClearColor;
}

ImGuiIO& MyImgui<SDL2_InHandler<Vulkan_Render>>::GetIO() {
    return ImGui::GetIO();
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::SetContext() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Control
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::SetStyle()
{
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    //ImGui::StyleColorsClassic();
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::InitVulkan()
{
    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForVulkan(m_InputHandler.GetWindow());
    m_VulkanInitInfo.Instance = m_InputHandler.GetRender().GetInstance();
    m_VulkanInitInfo.PhysicalDevice = m_InputHandler.GetRender().GetPhysicalDevice();
    m_VulkanInitInfo.Device = m_InputHandler.GetRender().GetDevice();
    m_VulkanInitInfo.QueueFamily = m_InputHandler.GetRender().GetQueueFamily();
    m_VulkanInitInfo.Queue = m_InputHandler.GetRender().GetQueue();
    m_VulkanInitInfo.PipelineCache = m_InputHandler.GetRender().GetPipelineCache();
    m_VulkanInitInfo.DescriptorPool = m_InputHandler.GetRender().GetDescriptorPool();
    m_VulkanInitInfo.RenderPass = m_MainWindowData.RenderPass;
    m_VulkanInitInfo.Subpass = 0;
    m_VulkanInitInfo.MinImageCount = m_MinImageCount;
    m_VulkanInitInfo.ImageCount = m_MainWindowData.ImageCount;
    m_VulkanInitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    m_VulkanInitInfo.Allocator = m_InputHandler.GetRender().GetAllocator();
    m_VulkanInitInfo.CheckVkResultFn = m_InputHandler.GetRender().GetCheckVkResultFunction();;
    ImGui_ImplVulkan_Init(&m_VulkanInitInfo);
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::NewFrame() {
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::ProcessEvent() {
    while(m_InputHandler.PollEvent())
    {
        const SDL_Event& l_Event = m_InputHandler.GetEvent();
        ImGui_ImplSDL2_ProcessEvent(&l_Event);
        if (l_Event.type == SDL_QUIT) {
            Done() = true;
        } 
        if (l_Event.type == SDL_WINDOWEVENT &&
            l_Event.window.event == SDL_WINDOWEVENT_CLOSE &&
            l_Event.window.windowID == m_InputHandler.GetWindowId(m_InputHandler.GetWindow())) {
            Done() = true;
        }
    }
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::FinishFrame() {
    // Rendering
    ImGui::Render();
    if (!IsMinimized())
    {
        SetClearColor();
        FrameRender(GetDrawData());
        FramePresent();
    }
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::ResizeSwapChain() {
    // Resize swap chain?
    int fb_width, fb_height;
    SDL_GetWindowSize(m_InputHandler.GetWindow(), &fb_width, &fb_height);
    if (fb_width > 0 && 
        fb_height > 0 && 
        (m_InputHandler.GetRender().GetSwapChainRebuild() ||
         m_MainWindowData.Width != fb_width || 
         m_MainWindowData.Height != fb_height))
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
        m_InputHandler.GetRender().GetSwapChainRebuild() = false;
    }
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::SetClearColor()
{
    m_MainWindowData.ClearValue.color.float32[0] = m_ClearColor.x * m_ClearColor.w;
    m_MainWindowData.ClearValue.color.float32[1] = m_ClearColor.y * m_ClearColor.w;
    m_MainWindowData.ClearValue.color.float32[2] = m_ClearColor.z * m_ClearColor.w;
    m_MainWindowData.ClearValue.color.float32[3] = m_ClearColor.w;
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::FrameRender(ImDrawData* draw_data)
{
    ImGui_ImplVulkanH_FrameSemaphores* FrameSemaphor = m_MainWindowData.FrameSemaphores;
    m_InputHandler.GetRender().AcquireNextImage(FrameSemaphor->ImageAcquiredSemaphore,
                                                m_MainWindowData.Swapchain,
                                                m_MainWindowData.FrameIndex);

    ImGui_ImplVulkanH_Frame* fd = &m_MainWindowData.Frames[m_MainWindowData.FrameIndex];
    m_InputHandler.GetRender().WaitForFences(fd->Fence);
    m_InputHandler.GetRender().ResetFences(fd->Fence);
    m_InputHandler.GetRender().BeginCommandBuffer(fd->CommandPool, fd->CommandBuffer);
    m_InputHandler.GetRender().BeginRenderPass(m_MainWindowData.RenderPass,
                                               fd->Framebuffer,
                                               m_MainWindowData.Width,
                                               m_MainWindowData.Height,
                                               m_MainWindowData.ClearValue,
                                               fd->CommandBuffer);
    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

    m_InputHandler.GetRender().EndRenderPass(fd->CommandBuffer);
    m_InputHandler.GetRender().EndCommandBuffer(fd->CommandBuffer);
    m_InputHandler.GetRender().QueueSubmit(FrameSemaphor->ImageAcquiredSemaphore,
                                           fd->CommandBuffer,
                                           FrameSemaphor->RenderCompleteSemaphore,
                                           fd->Fence);
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::FramePresent()
{
    auto semaphore = m_MainWindowData.FrameSemaphores[m_MainWindowData.SemaphoreIndex].RenderCompleteSemaphore;
    m_InputHandler.GetRender().PresentFrame(semaphore,
                                            m_MainWindowData.Swapchain,
                                            m_MainWindowData.FrameIndex);
    m_MainWindowData.SemaphoreIndex = (m_MainWindowData.SemaphoreIndex + 1) % 
                                       m_MainWindowData.SemaphoreCount; // Now we can use the next set of semaphores
}

bool MyImgui<SDL2_InHandler<Vulkan_Render>>::IsMinimized()
{
    ImDrawData* draw_data = GetDrawData();
    return (draw_data->DisplaySize.x <= 0.0f ||  draw_data->DisplaySize.y <= 0.0f); 
}

ImDrawData* MyImgui<SDL2_InHandler<Vulkan_Render>>::GetDrawData()
{
    return ImGui::GetDrawData();
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::WaitDeviceIdle()
{
    m_InputHandler.GetRender().WaitDeviceIdle();
}

void MyImgui<SDL2_InHandler<Vulkan_Render>>::Finish() {
    Done() = true;
}
