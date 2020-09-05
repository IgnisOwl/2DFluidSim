//Calculate everything relating to CFD
#include <vector>
#include <memory>
#include "Program.hpp"
#include "MathUtils.hpp"

using namespace std;


class Dynamics {
    public:
        Dynamics();
        ~Dynamics();
        //Converts the 1d matrix being used in this class to 2D vectors for easier graphical representation
        vector<vector<unique_ptr<float>>> getProcessedStep(vector<vector<unique_ptr<float>>>& gridIn);

        void simulationStep();

    private:
        /*SETTINGS*/
        const float timeSteps = 0.1;   //How fast the simulation runs
        const float diffusion = 1;    //How the velocity and vectors diffuse through the fluid
        const float viscosity = 1;    //Viscocity of the fluid
        const float iterations = 11;
        /*        */

        /*RUNTIME ARRAYS TODO: Dynamically allocated them on heap instead(with malloc)*/
        float* previousDensity;
        float* density;
        
        float* velocityY;
        float* previousVelocityY;

        float* velocityX;
        float* previousVelocityX;
        /*              */



        void addDensity(int x, int y, float amount);      //This function adds fluid desnsity to a specified location
        void addVelocity(int x, int y, float amountX, float amountY);      //This function adds fluid velocity to a specified location 

        //CONFUSING(I barely know how these work):
        void calculateDiffusion(int b, float* x, float* previousX, float diffusion, float timeSteps);
        void linearSolve(int b, float* x, float* previousX, float a, float c);
        void project(float *velocityX, float *velocityY, float *p, float *div);
        void advect(int b, float *density, float *previousDensity,  float *velocityX, float *velocityY, float timeSteps);
        void set_bnd(int b, float *x);
};