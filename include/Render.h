#pragma once
#include <vulkan/vulkan.h>


typedef struct {
    VkQueue queue;
    uint32_t familyIndex;
} VulkanQueue;


// Struct to Store all Vulkan-related data
typedef struct {
    VkInstance instance;
    VkPhysicalDevice pyhsical_device;
    VkPhysicalDeviceProperties pyhsical_device_properties;
    VkDevice device;
    VulkanQueue graphicsQueue;
} VulkanContext;


void Render_Init(void);
void Render_Destroy(void);