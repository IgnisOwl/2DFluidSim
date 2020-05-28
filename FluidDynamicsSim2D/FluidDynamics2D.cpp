//Tutorials used: http://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/, https://www.youtube.com/watch?v=QQzAHcojEKg&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx&index=1
//weird issue where you need to

#include <string>
#include <iostream>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#define windowX 600
#define windowY 400

void fixRenderBug() {
    //Weird issue where the renderer doesnt work unless you wait a few milliseconds before trying anything
    SDL_Delay(50);
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowX, windowY, SDL_WINDOW_SHOWN); //Create window object centered on screen(for both x and y)
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0); //Args: window object, index -1 to initialize the first driver one supporting the requested flags, flags(0 for no flags)

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

    fixRenderBug(); //Otherwise the first time it draws will be transparent(not a problem in a loop but just to be sure...)
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer); //clear the screen(draw with green)

    SDL_Delay(3000);

    return(0);

}

/*
void initializeGLEW() {
    #ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
    #endif
}

int main(int argc, char* argv[])
{LEW(); //Initialize glew to handle the sdl function calls
        
        SDL_Window* window;

        // Initialize SDL.
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                return 1;

        // Create the window where we will draw.
        window = SDL_CreateWindow("SDL_RenderClear",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        512, 512,
                        0);

SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
for(bool keep_running = true; keep_running;) {
  for(SDL_Event ev; SDL_PollEvent(&ev);) {
     if(ev.type == SDL_QUIT) {
        keep_running = false;
     }
  }
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}
}nitializeGLEW(); //Initialize glew to handle the sdl function calls
        
        SDL_Window* window;

        // Initialize SDL.
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
                return 1;

        // Create the window where we will draw.
        window = SDL_CreateWindow("SDL_RenderClear",
                        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                        512, 512,
                        0);

SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
for(bool keep_running = true; keep_running;) {
  for(SDL_Event ev; SDL_PollEvent(&ev);) {
     if(ev.type == SDL_QUIT) {
        keep_running = false;
     }
  }
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
}
}*/