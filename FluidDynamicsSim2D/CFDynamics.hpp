//Calculate everything relating to CFD
#include <vector>
#include <memory>
#include <time.h>     //For random
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "MathUtils.hpp"

using namespace std;


class Dynamics {

    public:
        Dynamics();
        vector<vector<unique_ptr<float>>> calculateStep(vector<vector<unique_ptr<float>>>& gridIn);
};