#include "CFDynamics.hpp"

using namespace std;

Dynamics::Dynamics() {
    //setup
}

vector<vector<unique_ptr<float>>> Dynamics::calculateStep(vector<vector<unique_ptr<float>>>& gridIn) {
    vector<vector<unique_ptr<float>>> gridOut = vector<vector<unique_ptr<float>>>();
    //Experimenting with turbulence
    for(int dim2i = 0; dim2i<gridIn.size(); dim2i++) {
        vector<unique_ptr<float>> dim2 = vector<unique_ptr<float>>();
        for(int dim1i = 0; dim1i<gridIn.at(dim2i).size(); dim1i++) {
            float oldVal = *gridIn.at(dim2i).at(dim1i);
            float newVal;

            //cout << oldVal << endl;

            //How high the number is changes the change of whether it will go up or down
            if(genRandom(0,1) == 0) {
                newVal = oldVal - 0.01F;
            }
            else {
                newVal = oldVal + 0.01F;
            }
            if(newVal<0.0F)
                newVal=0.0F;
            if(newVal>1.0F)
                newVal=1.0F;


            unique_ptr<float> dim1 = make_unique<float>(newVal);

            dim2.push_back(std::move(dim1));
        }
        gridOut.push_back(std::move(dim2));
    }
    return(gridOut);
}