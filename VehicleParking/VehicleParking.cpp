#include<bits/stdc++.h>
using namespace std;

class Parking{
    private:
        int floors;
        int numberOfParking;
        vector<vector<unordered_map<int, vector<string>>> parkingLot;

    public:
        Parking(int floors, int numberOfParking) : floors(floors), numberOfParking(numberOfParking) {
            parkingLot.resize(floors, vector<unordered_map<int, vector<string>>>(numberOfParking));
        }

};

int main(){
    return 0;
}