#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include "Render.h"
#include "Types.h"
#include "log.h"


SDL_Window* window = NULL;
VulkanContext* vkContext = NULL;


// Create Vulkan Instance
u8 initVkInstance(VulkanContext* context) {

    /*
    // Set VK Layers
    const char* enabledLayers[] = {
        "VK_LAYER_KHRONOS_validation",
    };*/


    // Getting Instance Extension, needed to find VK_Surface
    uint32_t instanceExtensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(window,&instanceExtensionCount,0);

    // Array of Instance Extension Names
    const char*  enabledInstanceExtensions[instanceExtensionCount];

    SDL_Vulkan_GetInstanceExtensions(window,&instanceExtensionCount,enabledInstanceExtensions);

    for(uint32_t i = 0; i < instanceExtensionCount;i++) {
        LOG_WRITE("INFO: VK_Instance_Extension %d: %s\n",i,enabledInstanceExtensions[i]);
    }

    // Application Info: Name,App Version, min. Vulkan Version required
    VkApplicationInfo applicationInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    applicationInfo.pApplicationName = "Nessi";
    applicationInfo.applicationVersion = VK_MAKE_VERSION(0,0,1);
    applicationInfo.apiVersion = VK_API_VERSION_1_0; 


    VkInstanceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &applicationInfo;
    createInfo.enabledExtensionCount = instanceExtensionCount;
    createInfo.ppEnabledExtensionNames = enabledInstanceExtensions;
  
 
    if(vkCreateInstance(&createInfo,0,&context->instance) != VK_SUCCESS) 
    {   
        LOG_WRITE("ERROR: Could not create vkCreateInstance!\n");
        return 0;
    }

    return 1;
}


// Find GPU(s) that support Vulkan
u8 getPhysicalDevice(VulkanContext* context) {
    // Numbers of GPUs that support Vulkan
    uint32_t device_count = 0;

    if(vkEnumeratePhysicalDevices(context->instance,&device_count,0) != VK_SUCCESS) 
    {   
        LOG_WRITE("ERROR: Could not find Physical Devices!\n");
        return 0;
    }

    
    if(!device_count) 
    {   
        LOG_WRITE("ERROR: Could not find Physical Devices!\n");
        return 0;
    }

    // List to store all GPUs that were found
    VkPhysicalDevice* devices = malloc(device_count * sizeof(VkPhysicalDevice));

    if(!devices) 
    {   
        LOG_WRITE("ERROR: Could not create devices list!\n");
        return 0; 
    }

    vkEnumeratePhysicalDevices(context->instance,&device_count,devices);

    LOG_WRITE("INFO: Found %d Vulkan GPU(s):\n",device_count);

    for (uint32_t i = 0; i < device_count; i++)
    {   
        VkPhysicalDeviceProperties properties = {0};
        vkGetPhysicalDeviceProperties(devices[i],&properties);
        LOG_WRITE("GPU %d: %s\n",i,properties.deviceName);
    }

    // Need to create a way to change this by the user!!!
    context->pyhsical_device = devices[0];

    vkGetPhysicalDeviceProperties(context->pyhsical_device,&context->pyhsical_device_properties);
    LOG_WRITE("INFO: Selected GPU: %s\n",context->pyhsical_device_properties.deviceName);

    free(devices);
    return 1;
}


// Create VKDevice
u8 createLogicalDevice(VulkanContext* context) {
    uint32_t queueFamiliesCount = 0;
    uint32_t graphicsQueueIndex = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(context->pyhsical_device,&queueFamiliesCount,0);

    if(!queueFamiliesCount) 
    {   
        LOG_WRITE("ERROR: No Vulkan Queues found!\n");
        return 0;
    }

    VkQueueFamilyProperties* queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * queueFamiliesCount);
    vkGetPhysicalDeviceQueueFamilyProperties(context->pyhsical_device,&queueFamiliesCount,queueFamilies);

    for(uint32_t i = 0; i < queueFamiliesCount;i++) {
        VkQueueFamilyProperties queueFamily = queueFamilies[i];

        if(queueFamily.queueCount > 0) 
        {
            if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
            {
                graphicsQueueIndex = i;
                break;
            }
        }
    }

    // Queue Priority set to Max
    const float priorities[] = {1.0f};


    VkDeviceQueueCreateInfo queueCreateInfo = {0};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = priorities;
    

    VkDeviceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pQueueCreateInfos = &queueCreateInfo;

    if(vkCreateDevice(context->pyhsical_device,&createInfo,0,&context->device) != VK_SUCCESS) 
    {   
        LOG_WRITE("ERROR: Failed to create Logical Device!\n");
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
    
    if(!context) 
    {   
        LOG_WRITE("ERROR: Could not allocate Memory for VulkanContext!\n");
        return NULL;
    }

    if(!initVkInstance(context)) 
    {   
        LOG_WRITE("ERROR: Could not create VKInstance!\n");
        return NULL;
    }

    if(!getPhysicalDevice(context)) 
    {   
        LOG_WRITE("ERROR: No GPU with Vulkan SUpport Found!\n");
        return NULL;
    }

    if(!createLogicalDevice(context)) 
    {
        LOG_WRITE("ERROR: Could not create Logical Device!\n");
        return NULL;
    }

    return context;
}


// Create SDL Window and initiate Vulkan
void Render_Init(void) {
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        LOG_WRITE("ERROR: Failed to initialize SDL!\n");
        exit(-1);
    }

    window = SDL_CreateWindow("Nessi",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,256, 240, SDL_WINDOW_VULKAN);

    if(!window) 
    {   
        LOG_WRITE("ERROR: Failed to create SDL Window!\n");
        exit(-1);
    }
    
    vkContext = initVulkan();
    
    if(!vkContext) 
    {   
        LOG_WRITE("ERROR: Failed to create VulkanContext!\n");
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
