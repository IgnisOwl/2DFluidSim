#include "CFDynamics.hpp"

using namespace std;

/*
*   THREE STEPS FOR FLUID SIMULATION:
*   
*   1) DIFFUSE - This basically *spreads out* the "highlighted liquid" amongst all the other liquid, like 
*             dye in a bowl of water, note that the velocity ALSO diffuses with this "highlighted fluid".
*             The Navier-Stokes equations is what calculates this.
*
*   2) PROJECT -  
*
*
*
*
*/
Dynamics::Dynamics() {
        //setup
        previousDensity = new float[tileRows*tileCols];
        density = new float[tileRows*tileCols];

        velocityY = new float[tileRows*tileCols];
        previousVelocityY = new float[tileRows*tileCols];

        velocityX = new float[tileRows*tileCols];
        previousVelocityX = new float[tileRows*tileCols];
}

Dynamics::~Dynamics() {
    delete previousDensity;
    delete density;

    delete velocityY;
    delete previousVelocityY;

    delete velocityX;
    delete previousVelocityX;
}

vector<vector<unique_ptr<float>>> Dynamics::calculateStep(vector<vector<unique_ptr<float>>>& gridIn) {
    vector<vector<unique_ptr<float>>> gridOut = vector<vector<unique_ptr<float>>>();
    return(gridOut);
}

void Dynamics::addDensity(int x, int y, float amount) {
    int index = cast_1D_2D(x, y);
    density[index] += amount;
}

void Dynamics::addVelocity(int x, int y, float amountX, float amountY) {
    int index = cast_1D_2D(x, y);
    density[index] += amountX;
    density[index] += amountY;
}

void calculateDIffusion(int b, float* x, float* previousX, float diffusion, float timeSteps) {
    float a = timeSteps * diffusion * (tileCols - 2) * (tileRows - 2);
    linearSolve(b, x, previousX, a, 1 + 6 * a, iterations, tileRows);
}
