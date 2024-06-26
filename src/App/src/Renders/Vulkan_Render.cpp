#include "Vulkan_Render.h"

#include <stdexcept>

Vulkan_Render::~Vulkan_Render()
{
    CleanupVulkan();
}

void Vulkan_Render::Assert(bool value, const std::string& message) {
    if (!value)
        throw std::runtime_error(message);
}

void Vulkan_Render::CleanupVulkan()
{
	vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);

#ifdef APP_USE_VULKAN_DEBUG_REPORT
	// Remove the debug report callback
	auto f_vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugReportCallbackEXT");
	f_vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif // APP_USE_VULKAN_DEBUG_REPORT

	vkDestroyDevice(g_Device, g_Allocator);
	vkDestroyInstance(g_Instance, g_Allocator);
}

void Vulkan_Render::CheckError(VkResult err)
{
	if (err == 0)
		return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0)
		abort();
}

VkAllocationCallbacks* Vulkan_Render::GetAllocator() {
	return g_Allocator;
}

VkInstance& Vulkan_Render::GetInstance() {
	return g_Instance;
}

VkPhysicalDevice& Vulkan_Render::GetPhysicalDevice() {
	return g_PhysicalDevice;
}

VkDevice& Vulkan_Render::GetDevice() {
	return g_Device;
}

uint32_t& Vulkan_Render::GetQueueFamily() {
	return g_QueueFamily;
}

VkQueue& Vulkan_Render::GetQueue() {
	return g_Queue;
}

VkDebugReportCallbackEXT& Vulkan_Render::GetDebugReport() {
	return g_DebugReport;
}

VkPipelineCache& Vulkan_Render::GetPipelineCache() {
	return g_PipelineCache;
}

VkDescriptorPool& Vulkan_Render::GetDescriptorPool() {
	return g_DescriptorPool;
}

VkSurfaceKHR& Vulkan_Render::GetSurface() {
    return surface;
}

bool Vulkan_Render::IsExtensionAvailable(const std::vector<VkExtensionProperties>& properties, const char* extension)
{
	for (const VkExtensionProperties& p : properties)
		if (strcmp(p.extensionName, extension) == 0)
			return true;
	return false;
}

VkPhysicalDevice Vulkan_Render::SetupVulkan_SelectPhysicalDevice()
{
    uint32_t gpu_count;
    VkResult err = vkEnumeratePhysicalDevices(GetInstance(), &gpu_count, nullptr);
    CheckError(err);
    Assert(gpu_count > 0, "gpu_count > 0");

    std::vector<VkPhysicalDevice> gpus;
    gpus.resize(gpu_count);
    err = vkEnumeratePhysicalDevices(GetInstance(), &gpu_count, gpus.data());
    CheckError(err);

    // If a number > 1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
    // most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
    // dedicated GPUs) is out of scope of this sample.
    for (VkPhysicalDevice& device : gpus)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(device, &properties);
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            return device;
    }

    // Use first GPU (Integrated) is a Discrete one is not available.
    if (gpu_count > 0)
        return gpus[0];
    return VK_NULL_HANDLE;
}

void Vulkan_Render::SetupVulkan(std::vector<const char*> instance_extensions)
{
    VkResult err;
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
    volkInitialize();
#endif

    // Create Vulkan Instance
    {
        VkInstanceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        // Enumerate available extensions
        uint32_t properties_count;
        std::vector<VkExtensionProperties> properties;
        vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
        properties.resize(properties_count);
        err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.data());
        CheckError(err);

        // Enable required extensions
        if (IsExtensionAvailable(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
            instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
        if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
        {
            instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        }
#endif

        // Enabling validation layers
#ifdef APP_USE_VULKAN_DEBUG_REPORT
        const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
        create_info.enabledLayerCount = 1;
        create_info.ppEnabledLayerNames = layers;
        instance_extensions.push_back("VK_EXT_debug_report");
#endif

        // Create Vulkan Instance
        create_info.enabledExtensionCount = (uint32_t)instance_extensions.size();
        create_info.ppEnabledExtensionNames = instance_extensions.data();
        err = vkCreateInstance(&create_info, GetAllocator(), &GetInstance());
        CheckError(err);
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
        volkLoadInstance(g_Instance);
#endif

        // Setup the debug report callback
#ifdef APP_USE_VULKAN_DEBUG_REPORT
        auto f_vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkCreateDebugReportCallbackEXT");
        IM_ASSERT(f_vkCreateDebugReportCallbackEXT != nullptr);
        VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
        debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        debug_report_ci.pfnCallback = debug_report;
        debug_report_ci.pUserData = nullptr;
        err = f_vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
        check_vk_result(err);
#endif
    }

    // Select Physical Device (GPU)
    GetPhysicalDevice() = SetupVulkan_SelectPhysicalDevice();

    // Select graphics queue family
    {
        uint32_t count;
        vkGetPhysicalDeviceQueueFamilyProperties(GetPhysicalDevice(), &count, nullptr);
        VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
        vkGetPhysicalDeviceQueueFamilyProperties(GetPhysicalDevice(), &count, queues);
        for (uint32_t i = 0; i < count; i++)
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                GetQueueFamily() = i;
                break;
            }
        free(queues);
        Assert(GetQueueFamily() != (uint32_t)-1, "GetQueueFamily() != (uint32_t)-1");
    }

    // Create Logical Device (with 1 queue)
    {
        std::vector<const char*> device_extensions;
        device_extensions.push_back("VK_KHR_swapchain");

        // Enumerate physical device extension
        uint32_t properties_count;
        std::vector<VkExtensionProperties> properties;
        vkEnumerateDeviceExtensionProperties(GetPhysicalDevice(), nullptr, &properties_count, nullptr);
        properties.resize(properties_count);
        vkEnumerateDeviceExtensionProperties(GetPhysicalDevice(), nullptr, &properties_count, properties.data());
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
        if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
            device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
#endif

        const float queue_priority[] = { 1.0f };
        VkDeviceQueueCreateInfo queue_info[1] = {};
        queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info[0].queueFamilyIndex = GetQueueFamily();
        queue_info[0].queueCount = 1;
        queue_info[0].pQueuePriorities = queue_priority;
        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
        create_info.pQueueCreateInfos = queue_info;
        create_info.enabledExtensionCount = (uint32_t)device_extensions.size();
        create_info.ppEnabledExtensionNames = device_extensions.data();
        err = vkCreateDevice(GetPhysicalDevice(), &create_info, GetAllocator(), &GetDevice());
        CheckError(err);
        vkGetDeviceQueue(GetDevice(), GetQueueFamily(), 0, &GetQueue());
    }

    // Create Descriptor Pool
    // The example only requires a single combined image sampler descriptor for the font image and only uses one descriptor set (for that)
    // If you wish to load e.g. additional textures you may need to alter pools sizes.
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 },
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1;
        pool_info.poolSizeCount = (uint32_t)ArraySize(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        err = vkCreateDescriptorPool(GetDevice(), &pool_info, GetAllocator(), &GetDescriptorPool());
        CheckError(err);
    }
}

void Vulkan_Render::WaitDeviceIdle()
{
    VkResult err = vkDeviceWaitIdle(GetDevice());
    CheckError(err);
}

void Vulkan_Render::PresentFrame(VkSemaphore& semaphore,
                                 const VkSwapchainKHR& swapchains,
                                 uint32_t& frameIndex)
{
    if (m_SwapChainRebuild)
        return;

    VkSemaphore render_complete_semaphore = semaphore;
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &render_complete_semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &swapchains;
    info.pImageIndices = &frameIndex;
    VkResult err = vkQueuePresentKHR(GetQueue(), &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        m_SwapChainRebuild = true;
        return;
    }
    CheckError(err);
}

bool& Vulkan_Render::GetSwapChainRebuild() {
    return m_SwapChainRebuild;
}

void Vulkan_Render::AcquireNextImage(VkSemaphore& imageAcquiredSemaphore,
                                     VkSwapchainKHR& swapchain,
                                     uint32_t& frameIndex)
{
    VkResult err = vkAcquireNextImageKHR(GetDevice(),
        swapchain,
        UINT64_MAX,
        imageAcquiredSemaphore,
        VK_NULL_HANDLE,
        &frameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR ||
        err == VK_SUBOPTIMAL_KHR)
    {
        m_SwapChainRebuild = true;
        return;
    }
    CheckError(err);
}

void Vulkan_Render::WaitForFences(VkFence& fence)
{
    VkResult err = vkWaitForFences(GetDevice(), 1, &fence, VK_TRUE, UINT64_MAX); // wait indefinitely instead of periodically checking
    CheckError(err);
}

void Vulkan_Render::ResetFences(VkFence& fence)
{
    VkResult err = vkResetFences(GetDevice(), 1, &fence);
    CheckError(err);
}

void Vulkan_Render::BeginCommandBuffer(VkCommandPool& commandPool, 
                                       VkCommandBuffer& commandBuffer)
{
    VkResult err = vkResetCommandPool(GetDevice(), commandPool, 0);
    CheckError(err);
    VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    err = vkBeginCommandBuffer(commandBuffer, &info);
    CheckError(err);
}

void Vulkan_Render::BeginRenderPass(VkRenderPass& renderPass,
                                    VkFramebuffer framebuffer,
                                    int width,
                                    int height,
                                    VkClearValue& clearValue,
                                    VkCommandBuffer& commandBuffer)
{
    VkRenderPassBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = renderPass;
    info.framebuffer = framebuffer;
    info.renderArea.extent.width = width;
    info.renderArea.extent.height = height;
    info.clearValueCount = 1;
    info.pClearValues = &clearValue;
    vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
}

void Vulkan_Render::EndRenderPass(VkCommandBuffer& commandBuffer)
{
    // Submit command buffer
    vkCmdEndRenderPass(commandBuffer);
}

void Vulkan_Render::EndCommandBuffer(VkCommandBuffer& commandBuffer)
{
    VkResult err = vkEndCommandBuffer(commandBuffer);
    CheckError(err);
}

void Vulkan_Render::QueueSubmit(VkSemaphore& imageAcquiredSemaphore,
                                VkCommandBuffer& commandBuffer, 
                                VkSemaphore& renderCompleteSemaphore,
                                VkFence& fence)
{
    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &imageAcquiredSemaphore;
    info.pWaitDstStageMask = &wait_stage;
    info.commandBufferCount = 1;
    info.pCommandBuffers = &commandBuffer;
    info.signalSemaphoreCount = 1;
    info.pSignalSemaphores = &renderCompleteSemaphore;
    VkResult err = vkQueueSubmit(GetQueue(), 1, &info, fence);
    CheckError(err);
}
