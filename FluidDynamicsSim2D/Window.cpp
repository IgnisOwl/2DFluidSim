#include "Window.hpp"

/*Program made by Connor Michalec with help from: https://mikeash.com/pyblog/fluid-simulation-for-dummies.html */

signed int initializeGLEW() {
    #ifndef __APPLE__
    glewExperimental = GL_TRUE;
    if(!glewInit()){
        return(-1);
    }
    return(0);
    #endif
}

int main(int argc, const char * argv[]) {
    Main run = Main();
}

Main::Main() {
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;  //calculated frame delay based on FPS
    Uint32 calculationStartTime;        //Calculation time marker, to calculate the final time to wait to get the fps to 60, as it will take some time to calculate appropraite things before frame render
    int calculationTime;                //time it takes to perform pre render calulations
    long countedFrames = 0;             //number of frames passed so far

    initialize();

    while(windowRunning) {
        calculationStartTime = SDL_GetTicks();      //how many ticks since initialization?


        handleEvents();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        /*Render the data*/
        dataRenderer.renderData();
        SDL_RenderPresent(renderer);
        /*               */
        

        calculationTime = SDL_GetTicks() - calculationStartTime;   //gets how long it took to handle events and stuff
        if(calculationTime < frameDelay)                           //If the calculations took less time then delay the amount of time to make it the target framerate, otherwise don't delay
        {
            //SDL_Delay(frameDelay - calculationTime);               //delay the amount of time it takes to make it 60fps minus the amount it took to perform calculations
        }

        currentFPS = countedFrames / (SDL_GetTicks() / 1000.0f);                    //calculated current FPS
        countedFrames++;

    } 

    cleanup();
}

void Main::initialize() {
    initializeGLEW();

    int flags;
    flags = 0;
    //flags = SDL_WINDOW_FULLSCREEN;

    if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow("Simple Hydrodynamics test simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            winSizeX, winSizeY, flags);
        if(!window) {
            // failed to create window
            std::cout << "Failed to init window." << std::endl;
        }


        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);    //GPU accelerated rendering: //https://dev.to/noah11012/using-sdl2-2d-accelerated-renderering-1kcb
        if(!renderer) {
            //failed to initialize renderer 
            std::cout << "Renderer failed to initialize";
        }
    }

    dataRenderer = Renderer(renderer); //recreate object using normal constructor
}

void Main::handleEvents() {
    //handle all events in queue
    SDL_Event event;
    while( SDL_PollEvent( &event ) != 0 ) {
        switch(event.type) {
        case SDL_QUIT:
            windowRunning = false;
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_f) {
                //Log fps in console for debugging
                cout << "FPS: " << currentFPS << endl; 
            }

        default:
            break;
        }
    }
}

void Main::cleanup() {
    /*exit and cleanup*/
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    /*              */
}

SDL_Renderer* Main::getRenderer() {
    return(renderer);
}