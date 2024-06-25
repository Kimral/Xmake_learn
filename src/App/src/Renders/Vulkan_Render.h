#pragma once

#include <string>
#include <vector>

#include "vulkan/vulkan.h"

class Vulkan_Render {
public:
	Vulkan_Render() = default;
	~Vulkan_Render();

    template<class Entity>
    int ArraySize(Entity& entity) {
        // Size of a static C-style array. Don't use on pointers!
        return (int)(sizeof(entity) / sizeof(*(entity)));
    }

    // Функция, возвращающая указатель на функцию check_vk_result
    void (*GetCheckVkResultFunction())(VkResult) {
        return &CheckError;
    }

    void Assert(bool value, const std::string& message);
	void CleanupVulkan();
	static void CheckError(VkResult err);

	VkAllocationCallbacks* GetAllocator();
	VkInstance& GetInstance();
	VkPhysicalDevice& GetPhysicalDevice();
	VkDevice& GetDevice();
	uint32_t& GetQueueFamily();
	VkQueue& GetQueue();
	VkDebugReportCallbackEXT& GetDebugReport();
	VkPipelineCache& GetPipelineCache();
	VkDescriptorPool& GetDescriptorPool();
	VkSurfaceKHR& GetSurface();

    bool IsExtensionAvailable(const std::vector<VkExtensionProperties>& properties, const char* extension);
    VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
    void SetupVulkan(std::vector<const char*> instance_extensions);
	void WaitDeviceIdle();
	void PresentFrame(VkSemaphore& semaphore,
					  const VkSwapchainKHR& swapchains,
					  uint32_t& frameIndex);
	bool& GetSwapChainRebuild();
	void AcquireNextImage(VkSemaphore& imageAcquiredSemaphore,
						  VkSwapchainKHR& swapchain,
					      uint32_t& frameIndex);
	void WaitForFences(VkFence& fence);
	void ResetFences(VkFence& fence);
	void BeginCommandBuffer(VkCommandPool& commandPool, VkCommandBuffer& commandBuffer);
	void BeginRenderPass(VkRenderPass& renderPass,
						 VkFramebuffer framebuffer,
						 int width,
						 int height,
						 VkClearValue& clearValue,
						 VkCommandBuffer& commandBuffer);
	void EndRenderPass(VkCommandBuffer& commandBuffer);
	void EndCommandBuffer(VkCommandBuffer& commandBuffer);
	void QueueSubmit(VkSemaphore& imageAcquiredSemaphore,
					 VkCommandBuffer& commandBuffer,
				     VkSemaphore& renderCompleteSemaphore,
					 VkFence& fence);

private:
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

	VkSurfaceKHR surface;

	bool m_SwapChainRebuild = false;
};