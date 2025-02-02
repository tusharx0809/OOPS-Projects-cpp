#include <bits/stdc++.h>
using namespace std;

class Parking
{
private:
    int floors;                                      // number of floors in multistorey parking
    int numberOfParking;                             // number of parkings on one storey
    vector<vector<pair<string, string>>> parkingLot; // 2d vector which represents multistorey parking, pair represents license plate and time at which the customer parked the vehicle

    /*  Structure of parking lot
        parkingLot = {
            floor 0: [
                parking spot 0 -> {"ABC123","10:00AM"}
                parking spot 1 -> {}
                parking spot 2 -> {}
            ],
            floor 1: [
                parking spot 0 -> {}
                parking spot 1 -> {}
                parking spot 2 -> {}
            ]
        }
    */

public:
    Parking(int floors, int numberOfParking) : floors(floors), numberOfParking(numberOfParking)
    {
        parkingLot.resize(floors, vector<pair<string, string>>(numberOfParking, {"", ""}));
        loadParkingLotFromFile("parking_lot.txt");
    }

    //function to get currentDateTime
    string getCurrentDateTime()
    {
        time_t timestamp;
        time(&timestamp);
        
        struct tm *local_time = localtime(&timestamp);

        int year = local_time->tm_year + 1900; // tm_year is years since 1900
        int month = local_time->tm_mon + 1;    // tm_mon is 0-11
        int day = local_time->tm_mday;         // tm_mday is day of the month (1-31)
        int hour = local_time->tm_hour;        // tm_hour is hour (0-23)
        int minute = local_time->tm_min;       // tm_min is minute (0-59)
        int second = local_time->tm_sec;       // tm_sec is second (0-59)

        string dateTime = to_string(month) + "-" + to_string(day) + "-" + to_string(year) + "," +
                      (hour < 10 ? "0" + to_string(hour) : to_string(hour)) + ":" +
                      (minute < 10 ? "0" + to_string(minute) : to_string(minute));

        return dateTime;
    }

    //function to get the parking duration of a vehicle
    long int getParkingTime(const string &parkingDateTime,const string &exitDateTime){
        string format = "%m-%d-%Y,%H:%M";

        tm tm1 = {};
        tm tm2 = {};

        istringstream ss1(exitDateTime);
        ss1 >> get_time(&tm1, format.c_str());

        istringstream ss2(parkingDateTime);
        ss2 >> get_time(&tm2, format.c_str());

        auto tp1 = chrono::system_clock::from_time_t(mktime(&tm1));
        auto tp2 = chrono::system_clock::from_time_t(mktime(&tm2));

        long int parkingTime = chrono::duration_cast<chrono::minutes>(tp2 - tp1).count();

        return parkingTime;
    }

    //Function to calculae parking charges per minute
    double calculateParkingCharges(int minutes){
        return minutes * 0.15;
    }

    //Function to add a vehicle at first empty parking spot
    bool parkVehicle(const string &licensePlate)
    {
        int spot = 0;
        int floor = 0;

        while (floor < floors)
        {
            if (parkingLot[floor][spot].first == "")
            {
                string parkingDateTime = getCurrentDateTime();
                parkingLot[floor][spot] = {licensePlate, parkingDateTime};
                saveParkingLotToFile("parking_lot.txt");
                cout << "Car Parked at floor:" << floor << " and spot: " << spot << endl;
                displayParkingLot();
                return true;
            }
            spot++;
            if (spot == numberOfParking)
            {
                floor++;
                spot = 0;
            }
        }

        cout << "Parking Full!" << endl;
        displayParkingLot();
        return false;
    }

    //Function to remove Vehicle from parking
    bool removeVehicle(const string &licensePlate)
    {
        int spot = 0;
        int floor = 0;

        while (floor < floors)
        {
            if (parkingLot[floor][spot].first == licensePlate)
            {
                string exitDateTime = getCurrentDateTime();
                long int totalParkingTime = getParkingTime(exitDateTime, parkingLot[floor][spot].second);
                double charges = calculateParkingCharges(totalParkingTime);

                parkingLot[floor][spot] = {"", ""};
                cout << "Car: " << licensePlate << " exited from floor:" << floor << " and spot: " << spot <<" at: "<<exitDateTime<<" Charges: "<< charges << endl;
                saveParkingLotToFile("parking_lot.txt");
                displayParkingLot();
                logChargesToFile(licensePlate, exitDateTime, totalParkingTime, charges,"charges.txt");
                return true;
            }
            spot++;
            if (spot == numberOfParking)
            {
                floor++;
                spot = 0;
            }
        }

        cout << "Car Not Found!" << endl;
        displayParkingLot();
        return false;
    }


    // Function to display the parking lot status
    void displayParkingLot()
    {
        for (int i = 0; i < floors; ++i)
        {
            cout << "Floor " << i << ":" << endl;
            for (int j = 0; j < numberOfParking; ++j)
            {
                cout << "  Spot " << j << ": ";
                if (parkingLot[i][j].first == "")
                {
                    cout << "Empty";
                }
                else
                {
                    cout << "Vehicle " << parkingLot[i][j].first << " entered at " << parkingLot[i][j].second;
                }
                cout << endl;
            }
        }
    }

    //Function to log charges info in txt file when vehicle exits from parking
    void logChargesToFile(const string & licensePlate,const string &exitDateTime, long int duration, double charges, const string &filename){
        ofstream file(filename, ios::app);

        if(!file.is_open()){
            cout<<"Error opening file!"<<endl;
            return;
        }
        file<<licensePlate<<" "<<exitDateTime<<" "<<duration<<" "<<charges<<endl;
        file.close(); 
    }

    //Function to save vehicle info in txt file when vehicle is entered into the parking lot
    void saveParkingLotToFile(const string &filename){
        ofstream file(filename);

        if(!file.is_open()){
            cout<<"Error Opening File!"<<endl;
            return;
        }

        for(int i=0;i<floors;i++){
            file << "Floor "<<i<<":"<<endl;
            for(int j=0;j<numberOfParking;j++){
                file<<"  Spot "<<j<<": ";
                if(parkingLot[i][j].first == ""){
                    file<<"Empty"<<endl;
                }else{
                    file<<"Vehicle "<<parkingLot[i][j].first <<" entered at "<<parkingLot[i][j].second << endl;
                }
            }
        }
        file.close();
        cout<<"Parking Lot Saved to "<<filename<<endl;
    }

    //Function to load the parking lot txt file when main function is executed
    void loadParkingLotFromFile(const string &filename){
        ifstream file(filename);

        if(!file.is_open()){
            cout<<"Error Opening File!"<<endl;
            return;
        }

        string line;
        int floor = -1;
        int spot = -1;

        while(getline(file, line)){
            if(line.find("Floor") != string::npos){
                floor++;
                spot = -1;
            }else if(line.find("Spot") != string::npos){
                spot++;
                size_t pos = line.find("Vehicle");
                if(pos != string::npos){
                    size_t time_pos = line.find(" entered at ");
                    string licensePlate = line.substr(pos+8, time_pos - (pos+8));
                    string time = line.substr(time_pos + 12);

                    parkingLot[floor][spot] = {licensePlate, time};
                }else{
                    parkingLot[floor][spot] = {"",""};
                }
            }
        }
        file.close();
        cout<<"Parking lot loaded from "<<filename<<endl;
    }
};


int main()
{
    Parking parking(2, 5); 

    // Vehicle will be parked wherever it finds the first empty spot parking
    parking.removeVehicle("PMN234");
    parking.removeVehicle("ABCD1234");
    parking.removeVehicle("LIN234");
    
    return 0;
}