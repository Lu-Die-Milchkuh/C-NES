#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include "Render.h"


SDL_Window* window = NULL;
VulkanContext* vkContext = NULL;


// Create Vulkan Instance
int initVkInstance(VulkanContext* context) {

    /*
    // Set VK Layers
    const char* enabledLayers[] = {
        "VK_LAYER_KHRONOS_validation",
    };*/

    VkApplicationInfo applicationInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    applicationInfo.pApplicationName = "Nessi";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(0,0,1);
    applicationInfo.apiVersion = VK_API_VERSION_1_0; 

    VkInstanceCreateInfo createInfo = {
        VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        0,
        0,
        &applicationInfo,
        0,
        0,
        0,
        0
    };
 
    if(vkCreateInstance(&createInfo,0,&context->instance) != VK_SUCCESS) {
        printf("Error: Could not create vkCreateInstance!\n");
        return 0;
    }

    return 1;
}


// Find GPU(s) that support Vulkan
int getPhysicalDevice(VulkanContext* context) {
    // Numbers of GPUs that support Vulkan
    unsigned int device_count = 0;

    if(vkEnumeratePhysicalDevices(context->instance,&device_count,0) != VK_SUCCESS) {
        printf("Error: Could not find Phsical Devices!\n");
        return 0;
    }

    
    if(!device_count) {
        printf("Error: Could not find Physical Devices!\n");
        return 0;
    }

    // List to store all GPUs that were found
    VkPhysicalDevice* devices = malloc(device_count * sizeof(VkPhysicalDevice));

    if(!devices) {
        printf("Error: Creating devices list!\n");
        return 0; 
    }

    vkEnumeratePhysicalDevices(context->instance,&device_count,devices);

    printf("Found %d Vulkan GPU(s):\n",device_count);

    for (unsigned int i = 0; i < device_count; i++)
    {   
        VkPhysicalDeviceProperties properties = {};
        vkGetPhysicalDeviceProperties(devices[i],&properties);
        printf("GPU %d: %s\n",i,properties.deviceName);
    }

    // Need to create a way to change this by the user!!!
    context->pyhsical_device = devices[0];

    vkGetPhysicalDeviceProperties(context->pyhsical_device,&context->pyhsical_device_properties);
    printf("Selected GPU: %s\n",context->pyhsical_device_properties.deviceName);

    free(devices);
    return 1;
}


// Create VKDevice
int createLogicalDevice(VulkanContext* context) {
    unsigned int queueFamiliesCount = 0;
    unsigned int graphicsQueueIndex = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(context->pyhsical_device,&queueFamiliesCount,0);

    if(!queueFamiliesCount) {
        printf("Error: No Queues found!");
        return 0;
    }

    VkQueueFamilyProperties* queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * queueFamiliesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(context->pyhsical_device,&queueFamiliesCount,queueFamilies);

    for(unsigned int i = 0; i < queueFamiliesCount;i++) {
        VkQueueFamilyProperties queueFamily = queueFamilies[i];

        if(queueFamily.queueCount > 0) {
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                graphicsQueueIndex = i;
                break;
            }
        }
    }

    // Queue Priority set to Max
    float priorities[] = {1.0f};

    VkDeviceQueueCreateInfo queueCreateInfo = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        0,
        0,
        graphicsQueueIndex,
        1,
        priorities, 
    };

    //VkPhysicalDeviceFeatures enabledFeatures = {};

    VkDeviceCreateInfo createInfo = { 
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        0,
        0,
        1,
        &queueCreateInfo,
        0,0,0,0,0 
    };
    

    if(vkCreateDevice(context->pyhsical_device,&createInfo,0,&context->device) != VK_SUCCESS) {
        printf("Error:  Failed to create Logical Device\n");
        return 0;
    }

    context->graphicsQueue.familyIndex = graphicsQueueIndex; 
    vkGetDeviceQueue(context->device,graphicsQueueIndex,0,&context->graphicsQueue.queue);

    free(queueFamilies);
    return 1;
}


// Create Vulkan Context
VulkanContext* initVulkan(void) {
    VulkanContext* context = malloc(sizeof(VulkanContext));
    
    if(!context) {
        printf("Error reserving Memory for VulkanContext!\n");
        return NULL;
    }

    if(!initVkInstance(context)) {
        printf("Error: Could not create VKInstance!\n");
        return NULL;
    }

    if(!getPhysicalDevice(context)) {
        printf("Error: Could not find any Device with Vulkan Support!\n");
        return NULL;
    }

    if(!createLogicalDevice(context)) {
        printf("Error: Could not create Logical Device!\n");
        return NULL;
    }

    return context;
}


// Create SDL Window and initiate Vulkan
void Render_Init(void) {
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Error: initializing SDL!\n");
        exit(-1);
    }

    window = SDL_CreateWindow("Nessi",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,256, 240, SDL_WINDOW_VULKAN);

    if(!window) 
    {
        printf("Error: creating SDL Window!\n");
        exit(-1);
    }
    
    vkContext = initVulkan();
    
    if(!vkContext) {
        printf("Error: Failed to create VulkanContext!\n");
        exit(-1);
    }
    
}


// Clear up every SDL and Vulkan related stuff
void Render_Destroy(void) {
    vkDeviceWaitIdle(vkContext->device);    // Wait till Device is in idle state
    vkDestroyDevice(vkContext->device,0);  
    vkDestroyInstance(vkContext->instance,0);   
    free(vkContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
