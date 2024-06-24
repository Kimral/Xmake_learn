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
        return &check_vk_result;
    }

    void Assert(bool value, const std::string& message);
	void CleanupVulkan();
	static void check_vk_result(VkResult err);

	VkAllocationCallbacks* GetAllocator();
	VkInstance& GetInstance();
	VkPhysicalDevice& GetPhysicalDevice();
	VkDevice& GetDevice();
	uint32_t& GetQueueFamily();
	VkQueue& GetQueue();
	VkDebugReportCallbackEXT GetDebugReport();
	VkPipelineCache GetPipelineCache();
	VkDescriptorPool& GetDescriptorPool();

    bool IsExtensionAvailable(const std::vector<VkExtensionProperties>& properties, const char* extension);
    VkPhysicalDevice SetupVulkan_SelectPhysicalDevice();
    void SetupVulkan(std::vector<const char*> instance_extensions);

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
};