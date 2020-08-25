#pragma once

#include <vector>
#include <memory>
#include <math.h>

using namespace std;

/* ------------- 2D VECTOR/MATRIX NORMALIZATION ------------ */
//Default is float in int out
static vector<vector<unique_ptr<int>>> normalize2DMatrix(vector<vector<unique_ptr<float>>>& in, float inMin, float inMax, int outMin, int outMax) {    //Pass simulationgrid as reference, as the only way otherwise would be to move the data in which would erase it from original memory
    vector<vector<unique_ptr<int>>> out = vector<vector<unique_ptr<int>>>();


    /*iterate through the vector and change each value accordingly*/
    for(int dim2i = 0; dim2i<in.size(); dim2i++) {
        vector<unique_ptr<int>> dim2 = vector<unique_ptr<int>>();

        for(int dim1i = 0; dim1i<in.at(dim2i).size(); dim1i++) {
            float oldVal = *in.at(dim2i).at(dim1i);

            /*NOTE: This is probably not a very efficient way of doing this,
            * As I came up with this formula myself, in the future replace with a more
            * efficient one.
            */
            unique_ptr<int> dim1 = make_unique<int>(round((oldVal*((float)outMax-(float)outMin))/(inMax-inMin)));

            dim2.push_back(std::move(dim1));
        }                                                                                       

        out.push_back(std::move(dim2));
    }
    /*              */

    return(out);
}

//Overloaded for float out
static vector<vector<unique_ptr<float>>> normalize2DMatrix(vector<vector<unique_ptr<float>>>& in, float inMin, float inMax, float outMin, float outMax) {    
    vector<vector<unique_ptr<float>>> out = vector<vector<unique_ptr<float>>>();


    /*iterate through the vector and change each value accordingly*/
    for(int dim2i = 0; dim2i<in.size(); dim2i++) {
        vector<unique_ptr<float>> dim2 = vector<unique_ptr<float>>();

        for(int dim1i = 0; dim1i<in.at(dim2i).size(); dim1i++) {
            float oldVal = *in.at(dim2i).at(dim1i);

            /*NOTE: This is probably not a very efficient way of doing this,
            * As I came up with this formula myself, in the future replace with a more
            * efficient one.
            */
            unique_ptr<float> dim1 = make_unique<float>((oldVal*(outMax-outMin))/(inMax-inMin));

            dim2.push_back(std::move(dim1));
        }                                                                                       

        out.push_back(std::move(dim2));
    }
    /*              */

    return(out);
}

/* -----------------                        ---------------------- */

