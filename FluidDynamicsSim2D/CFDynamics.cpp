#include "CFDynamics.hpp"

using namespace std;

Dynamics::Dynamics() {
    //setup
    srand(time(0));
}

vector<vector<unique_ptr<float>>> Dynamics::calculateStep(vector<vector<unique_ptr<float>>>& gridIn) {
    vector<vector<unique_ptr<float>>> gridOut = vector<vector<unique_ptr<float>>>();
    //Experimenting with turbulence
    for(int dim2i = 0; dim2i<gridIn.size(); dim2i++) {
        vector<unique_ptr<float>> dim2 = vector<unique_ptr<float>>();
        for(int dim1i = 0; dim1i<gridIn.at(dim2i).size(); dim1i++) {
            float oldVal = *gridIn.at(dim2i).at(dim1i);
            float newVal;
            if(((rand() % 2) + 0) == 1) { //random value between 0 and 1 so 1/2 chance
                newVal = oldVal - ((((float) (rand() % 100)) + 0));
            }
            else {
                newVal = oldVal + ((((float) (rand() % 100)) + 0));
            }
            unique_ptr<float> dim1 = make_unique<float>(newVal);

            dim2.push_back(std::move(dim1));
        }
        gridOut.push_back(std::move(dim2));
    }
    
    return(normalize2DMatrix(gridOut, 0, 100, 0.0F, 1.0F));
}