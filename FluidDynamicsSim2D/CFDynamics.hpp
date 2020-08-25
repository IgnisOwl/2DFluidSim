//Calculate everything relating to CFD
#include <vector>
#include <memory>
#include "Program.hpp"
#include "MathUtils.hpp"

using namespace std;


class Dynamics {
    public:
        Dynamics();
        vector<vector<unique_ptr<float>>> calculateStep(vector<vector<unique_ptr<float>>>& gridIn);


    private:
        /*SETTINGS*/
        const float timeSteps = 0.1;   //How fast the simulation runs
        const float diffusion = 0;    //How the velocity and vectors diffuse through the fluid
        const float viscosity = 0;    //Viscocity of the fluid
        const float iterations = 10;
        /*        */

        /*RUNTIME ARRAYS*/
        float* previousDensity;
        float* density;
        
        float* velocityY;
        float* previousVelocityY;

        float* velocityX;
        float* previousVelocityX;
        /*              */

        void addDensity(int x, int y, float amount);      //This function adds fluid desnsity to a specified location
        void addVelocity(int x, int y, float amountX, float amountY);      //This function adds fluid velocity to a specified location 
        void calculateDiffusion(int b, float* x, float* previousX, float diffusion, float timeSteps);
};