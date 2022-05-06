#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


// Create SDL Window
void RENDER_INIT() {
    
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("Error initializing SDL!");
        exit(-1);
    }

    window = SDL_CreateWindow("C_NES",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1000, 1000, 0);

    if(window == NULL) 
    {
        printf("Error creating Window!");
        exit(-1);
    }

    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    if(renderer == NULL) 
    {
        printf("Error creating Renderer!");
        exit(-1);
    }


}


void RENDER_TEXTURE(SDL_Texture* texture,int x, int y) {}


// Clear up every SDL related stuff
void RENDER_DESTROY() {
    
    SDL_DestroyWindow(window);
    SDL_Quit();
}
