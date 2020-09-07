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
 *                   be equal to the amount coming up, this is a setup stage to put the system into equilibrium. Basically it
 *                   cleans up and makes sure it's the same amount of fluid everywhere.
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
 *   6) SIMULATIONSTEP - This function puts all of this together and actually calculates each "step" in the simulation.
 *                    
 */
Dynamics::Dynamics() {
        //setup, store on heap
        previousDensity = new float[tileRows*tileCols];
        density = new float[tileRows*tileCols];

        velocityY = new float[tileRows*tileCols];
        previousVelocityY = new float[tileRows*tileCols];

        velocityX = new float[tileRows*tileCols];
        previousVelocityX = new float[tileRows*tileCols];
}

Dynamics::~Dynamics() {
    delete[] previousDensity;
    delete[] density;

    delete[] velocityY;
    delete[] previousVelocityY;

    delete[] velocityX;
    delete[] previousVelocityX;
}

void Dynamics::setProcessedSteps(vector<vector<unique_ptr<float>>>& gridIn) {
    for(int rowi = 0; rowi<gridIn.size(); rowi++) {
        vector<unique_ptr<float>> row;

        for(int coli = 0; coli<gridIn.at(rowi).size(); coli++) {
            row.push_back(make_unique<float>(density[cast_1D_2D(coli, rowi)]));
            //cout << sizeof(density)/sizeof(density[0]) << endl;
            //    //No change
            //   row.push_back(move(gridIn.at(rowi).at(coli)));
        }
        gridIn.at(rowi) = move(row);
    }
}

void Dynamics::addDensity(int x, int y, float amount) {
    int index = cast_1D_2D(x, y);
    density[index] += amount;
}

void Dynamics::addVelocity(int x, int y, float amountX, float amountY) {
    //Im unsure why the x and ys are flipped
    int index = cast_1D_2D(x, y);
    velocityX[index] += amountY;
    velocityY[index] += amountX;
}

void Dynamics::calculateDiffusion(int b, float* x, float* previousX, float diffusion, float timeSteps) {
    float a = timeSteps * diffusion * (tileCols - 2) * (tileRows - 2);
    linearSolve(b, x, previousX, a, 1 + 6 * a);
}

//This is the main function and will be called to progress the simulation
void Dynamics::simulationStep() {
    //OLD "this" is a pointer
    
    calculateDiffusion(1, previousVelocityX, velocityX, viscosity, timeSteps);
    calculateDiffusion(2, previousVelocityY, velocityY, viscosity, timeSteps);
    
    project(previousVelocityX, previousVelocityY, velocityX, velocityY);
    
    advect(1, velocityX, previousVelocityX, previousVelocityX, previousVelocityY, timeSteps);
    advect(2, velocityY, previousVelocityY, previousVelocityX, previousVelocityY, timeSteps);
    
    project(velocityX, velocityY, previousVelocityX, previousVelocityY);
    
    calculateDiffusion(0, previousDensity, density, diffusion, timeSteps);
    advect(0, density, previousDensity, velocityX, velocityY, timeSteps);
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
    linearSolve(0, p, div, 1, 4);
    
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
}

void Dynamics::advect(int b, float *density, float *previousDensity, float *velocityX, float *velocityY, float timeSteps) {
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
                s0 * ( t0 * previousDensity[cast_1D_2D(i0i, j0i)] + t1 * previousDensity[cast_1D_2D(i0i, j1i)]) +
                s1 * ( t0 * previousDensity[cast_1D_2D(i1i, j0i)] + t1 * previousDensity[cast_1D_2D(i1i, j1i)]);
        }
    }
    set_bnd(b, density);
}


void Dynamics::set_bnd(int b, float *x)
{
    //Y:
    for(int i = 1; i < tileRows - 1; i++) {
        x[cast_1D_2D(i, 0)] = b == 2 ? -x[cast_1D_2D(i, 1)] : x[cast_1D_2D(i, 1)];

        //Ternary operator, set cast_1D_2D(i, tileRows-1, k)] to -x[cast_1D_2D(i, tileRows-2, k)]- if b = 2 otherwise set it to x[cast_1D_2D(i, tileRows-2, k)]
        x[cast_1D_2D(i, tileRows-1)] = b == 2 ? -x[cast_1D_2D(i, tileRows-2)] : x[cast_1D_2D(i, tileRows-2)];
    }

    //X:
    for(int j = 1; j < tileCols - 1; j++) {
        x[cast_1D_2D(0  , j)] = b == 1 ? -x[cast_1D_2D(1  , j)] : x[cast_1D_2D(1  , j)];
        x[cast_1D_2D(tileCols-1, j)] = b == 1 ? -x[cast_1D_2D(tileCols-2, j)] : x[cast_1D_2D(tileCols-2, j)];
    }

    //I'm assuming each of these is for each border
    
    x[cast_1D_2D(0, 0)] = 0.33f * (x[cast_1D_2D(1, 0)] + x[cast_1D_2D(0, 1)]);

    x[cast_1D_2D(0, tileRows-1)] = 0.33f * (x[cast_1D_2D(1, tileRows-1)] + x[cast_1D_2D(0, tileRows-2)]);

    x[cast_1D_2D(tileCols-1, 0)] = 0.33f * (x[cast_1D_2D(tileCols-2, 0)] + x[cast_1D_2D(tileCols-1, 1)]);

    x[cast_1D_2D(tileCols-1, tileRows-1)] = 0.33f * (x[cast_1D_2D(tileCols-2, tileRows-1)] + x[cast_1D_2D(tileCols-1, tileRows-2)]);
}