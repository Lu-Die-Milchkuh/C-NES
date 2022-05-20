#pragma once
#include <vulkan/vulkan.h>


// Struct to Store all Vulkan-related data
typedef struct {
    VkInstance instance;
} VulkanContext;

VulkanContext* initVulkan(void);

void Render_Init(void);
void Render_Destroy(void);