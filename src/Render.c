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
        printf("Error vkCreateInstance!\n");
        return 0;
    }

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
        printf("Couldt not create VKInstance!\n");
        return NULL;
    }

    return context;
}


// Create SDL Window
void Render_Init() {
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Error initializing SDL!\n");
        exit(-1);
    }

    window = SDL_CreateWindow("Nessi",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,256, 240, SDL_WINDOW_VULKAN);

    if(!window) 
    {
        printf("Error creating SDL Window!\n");
        exit(-1);
    }
    
    vkContext = initVulkan();
    
    if(!vkContext) {
        printf("Failed to create VulkanContext!\n");
        exit(-1);
    }


}


// Clear up every SDL related stuff
void Render_Destroy() {
    SDL_DestroyWindow(window);
    free(vkContext);
    SDL_Quit();
}
