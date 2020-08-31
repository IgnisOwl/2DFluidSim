#include "CFDynamics.hpp"

using namespace std;

/*
 *   PARTS OF THE FLUID SIMULATION:
 *   
 *   1) DIFFUSE - This basically *spreads out* the "highlighted liquid" amongst all the other liquid, like 
 *                   dye in a bowl of water, note that the velocity ALSO diffuses with this "highlighted fluid".
 *                   The Navier-Stokes equations is what calculates this.
 *
 *   2) PROJECT -  This is tied to the concept of the fluid being incompressible, so the amount of fluid going in has to
 *                   be equal to the amount coming up, this is a setup stage to put the system into equilibrium.
 *
 *   3) LINEAR SOLVING - This solves a linear diffrential equation and i have no idea how it works or anything the code is just stolen
 *
 *   4) ADVECTION - This is responsible for moving the fluid around, it looks at each cell iteratively and it takes the velocity,
 *                    comapres it with previous velocity(or it follows it back in time), and it sees where it lands. It then takes a weighted average(each observation has a frequnecy/weight assigned to it)
 *                    of the cells around the spot where it lands and applies tat value to the current cell, so it basically is allowing cells to push velocities to other cells. Note that diffusion is the motion of the fluid spreading
 *                    out, advection is the motion associated with velocities.
 * 
 *   5) SET_BND - This handles the edge and corner cells and how they interact/bounce. The fluid needs to become "Mirrored"
 *                  The b variable tells us what "wall" we are at
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

void Dynamics::calculateDiffusion(int b, float* x, float* previousX, float diffusion, float timeSteps) {
    float a = timeSteps * diffusion * (tileCols - 2) * (tileRows - 2);
    linearSolve(b, x, previousX, a, 1 + 6 * a);
}

//Basically what i understand is happen here is its saying "The value of the new cell is based on a function of itself and all of its neighbors"
void Dynamics::linearSolve(int b, float* x, float* previousX, float a, float c) {
    float cRecip = 1.0 / c;
    for (int k = 0; k < iterations; k++) {
            for (int j = 1; j < tileRows - 1; j++) {    //For y
                for (int i = 1; i < tileCols - 1; i++) {       //For x
                    x[cast_1D_2D(i, j)] =               //It is equal to it's old value + some combination of its current values
                        (previousX[cast_1D_2D(i, j)]
                            + a*(    x[cast_1D_2D(i+1, j)]
                                    +x[cast_1D_2D(i-1, j)]
                                    +x[cast_1D_2D(i, j+1)]
                                    +x[cast_1D_2D(i, j-1)]
                           )) * cRecip;
                }
            }
        set_bnd(b, x);
    }
}

//Project function
void Dynamics::project(float *velocityX, float *velocityY, float *p, float *div) {
        for (int j = 1; j < tileRows - 1; j++) {
            for (int i = 1; i < tileCols - 1; i++) {
                div[cast_1D_2D(i, j)] = -0.5f*(
                         velocityX[cast_1D_2D(i+1, j)]
                        -velocityX[cast_1D_2D(i-1, j)]
                        +velocityY[cast_1D_2D(i  , j+1)]
                        -velocityY[cast_1D_2D(i  , j-1)]
                    )/tileCols;
                p[cast_1D_2D(i, j)] = 0;
            }
        }

    set_bnd(0, div); 
    set_bnd(0, p);

    linearSolve(0, p, div, 1, 6);
    
    for (int j = 1; j < tileRows - 1; j++) {
        for (int i = 1; i < tileCols - 1; i++) {
            velocityX[cast_1D_2D(i, j)] -= 0.5f * (  p[cast_1D_2D(i+1, j)]
                                            -p[cast_1D_2D(i-1, j)]) * tileCols;
            velocityY[cast_1D_2D(i, j)] -= 0.5f * (  p[cast_1D_2D(i, j+1)]
                                            -p[cast_1D_2D(i, j-1)]) * tileRows;
        }
    }

    set_bnd(1, velocityX);
    set_bnd(2, velocityY);
    set_bnd(3, velocityZ);
}

void Dynamics::advect(int b, float *density, float *previousDensity,  float *velocityX, float *velocityY) {
    float i0, i1, j0, j1; //Index and previous index
    
    float dtx = timeSteps * (tileCols - 2);
    float dty = timeSteps * (tileRows - 2);
    
    float s0, s1, t0, t1;
    float tmp1, tmp2, x, y;
    
    float Nfloat = tileCols;
    float ifloat, jfloat;
    int i, j;
    
    for(j = 1, jfloat = 1; j < tileRows - 1; j++, jfloat++) { 
        for(i = 1, ifloat = 1; i < tileCols - 1; i++, ifloat++) {
            tmp1 = dtx * velocityX[cast_1D_2D(i, j)];
            tmp2 = dty * velocityY[cast_1D_2D(i, j)];
            x    = ifloat - tmp1; 
            y    = jfloat - tmp2;
            
            if(x < 0.5f) x = 0.5f; 
            if(x > Nfloat + 0.5f) x = Nfloat + 0.5f; 
            i0 = floorf(x); 
            i1 = i0 + 1.0f;
            if(y < 0.5f) y = 0.5f; 
            if(y > Nfloat + 0.5f) y = Nfloat + 0.5f; 
            j0 = floorf(y);
            j1 = j0 + 1.0f; 
            
            s1 = x - i0; 
            s0 = 1.0f - s1; 
            t1 = y - j0; 
            t0 = 1.0f - t1;
            
            int i0i = i0;
            int i1i = i1;
            int j0i = j0;
            int j1i = j1;
            
            density[cast_1D_2D(i, j)] = 
            
                s0 * ( t0 * previousDensity[cast_1D_2D(i0i, j0i)])
                    +( t1 * previousDensity[cast_1D_2D(i0i, j1i)])
                +s1 * ( t0 * previousDensity[cast_1D_2D(i1i, j0i)])
                    +( t1 * previousDensity[cast_1D_2D(i1i, j1i)]);
        }
    }
    set_bnd(b, density);
}