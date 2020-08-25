#include <vector>
#include <memory>

using namespace std;

class Dynamics {
    Dynamics() {
        //setup
    }

    vector<vector<unique_ptr<float>>> calculateStep(vector<vector<unique_ptr<float>>>& gridIn) {
        vector<vector<unique_ptr<float>>> gridOut = vector<vector<unique_ptr<float>>>();
        return(gridOut);
    }
}; 