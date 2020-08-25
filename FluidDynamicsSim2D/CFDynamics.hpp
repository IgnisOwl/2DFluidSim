//Calculate everything relating to CFD
#include <vector>
#include <memory>

using namespace std;


class Dynamics {
    public:
        Dynamics();
        vector<vector<unique_ptr<float>>> calculateStep(vector<vector<unique_ptr<float>>>& gridIn);
};