#include <bits/stdc++.h>
using namespace std;
// ------------------------- VEHICLE CLASS -------------------------- //

class Vehicle
{
protected:
    string licensePlate;
    string entryTime;

public:
    Vehicle(string licensePlate, string entryTime) : licensePlate(licensePlate), entryTime(entryTime) {}
    

    virtual ~Vehicle() {}

    virtual double getChargesPerMinute() const = 0; //virtual function of vehicle to calculate charges per minute

    string getLicensePlate() const { return licensePlate; }
    string getEntryTime() const { return entryTime; }
};


// ------------------------- CAR CLASS -------------------------- //

class Car : public Vehicle{
    public:
    Car(string licensePlate, string entryTime) : Vehicle(licensePlate, entryTime) {}

    double getChargesPerMinute() const override {
        return 0.2;
    }
};

// ------------------------- BIKE CLASS -------------------------- //

class Bike : public Vehicle{
    public:
    Bike(string licensePlate, string entryTime) : Vehicle(licensePlate, entryTime) {}

    double getChargesPerMinute() const override {
        return 0.1;
    }
};

// ------------------------- PARKING CLASS -------------------------- //
class Parking
{
private:
    int floors;                                               // number of floors in multistorey parking
    int numberOfParking;                                      // number of parkings on one storey
    vector<vector<Vehicle *>> parkingLot; // 2d vector which represents multistorey parking, tuple represents license plate and time at which the customer parked the vehicle

public:
    Parking(int floors, int numberOfParking) : floors(floors), numberOfParking(numberOfParking)
    {
        parkingLot.resize(floors, vector<Vehicle *>(numberOfParking, nullptr));
        loadParkingLotFromFile("parking_lot.txt");
    }

    ~Parking(){
        for(int floor = 0; floor < floors; floor++){
            for(int spot = 0; spot < numberOfParking; spot++){
                if(parkingLot[floor][spot] == nullptr){
                    delete parkingLot[floor][spot];
                }
            }
        }
    }

    // function to get currentDateTime
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

        string dateTime = to_string(month) + "-" + to_string(day) + "-" + to_string(year) + "," +
                          (hour < 10 ? "0" + to_string(hour) : to_string(hour)) + ":" +
                          (minute < 10 ? "0" + to_string(minute) : to_string(minute));

        return dateTime;
    }

    // function to get the parking duration of a vehicle
    long int getParkingTime(const string &parkingDateTime, const string &exitDateTime)
    {
        string format = "%m-%d-%Y,%H:%M";

        tm tm1 = {};
        tm tm2 = {};

        istringstream ss2(exitDateTime);
        ss2 >> get_time(&tm2, format.c_str());

        istringstream ss1(parkingDateTime);
        ss1 >> get_time(&tm1, format.c_str());

        auto tp1 = chrono::system_clock::from_time_t(mktime(&tm1));
        auto tp2 = chrono::system_clock::from_time_t(mktime(&tm2));

        long int parkingTime = chrono::duration_cast<chrono::minutes>(tp2 - tp1).count();

        return parkingTime;
    }


    // Function to add a vehicle at the first empty parking spot
    bool parkVehicle(Vehicle *vehicle)
    {
        int spot = 0;
        int floor = 0;

        while (floor < floors)
        {
            if (parkingLot[floor][spot] == nullptr)
            {
                
                parkingLot[floor][spot] = vehicle;
                saveParkingLotToFile("parking_lot.txt");
                cout << parkingLot[floor][spot]->getLicensePlate() << " parked at floor: " << floor << " and spot: " << spot << endl;
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

    // Function to remove Vehicle from parking
    bool removeVehicle(const string &licensePlate)
    {
        int spot = 0;
        int floor = 0;

        while (floor < floors)
        {
            
            if (parkingLot[floor][spot] != nullptr && parkingLot[floor][spot]->getLicensePlate() == licensePlate)
            {
                string exitDateTime = getCurrentDateTime();
                long int totalParkingTime = getParkingTime(parkingLot[floor][spot]->getEntryTime(), exitDateTime);
                double charges = totalParkingTime * parkingLot[floor][spot]->getChargesPerMinute();

                cout << licensePlate << " exited from floor:" << floor << " and spot: " << spot << " at: " << exitDateTime << " Charges Rs: " << charges << endl;
                logChargesToFile(licensePlate, exitDateTime, totalParkingTime, charges, "charges.txt");
                delete parkingLot[floor][spot];
                parkingLot[floor][spot] = nullptr;
                saveParkingLotToFile("parking_lot.txt");
                displayParkingLot(); // Now called only once   
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
        for (int floor = 0; floor < floors; floor++)
        {
            cout << "Floor " << floor << ":" << endl;
            for (int spot = 0; spot < numberOfParking; spot++)
            {
                cout << "  Spot " << spot << ": ";
                if (parkingLot[floor][spot] == nullptr)
                {
                    cout << "Empty";
                }
                else
                {
                    cout << "Vehicle " << parkingLot[floor][spot]->getLicensePlate()
                         << " type " << (dynamic_cast<Car *>(parkingLot[floor][spot]) ? "Car" : "Bike")
                         << " entered at " << parkingLot[floor][spot]->getEntryTime(); 
            }
            cout << endl;
        }
    }
    }

    // Function to log charges info in txt file when vehicle exits from parking
    void logChargesToFile(const string &licensePlate, const string &exitDateTime, long int duration, double charges, const string &filename)
    {
        ofstream file(filename, ios::app);

        if (!file.is_open())
        {
            cout << "Error opening file!" << endl;
            return;
        }
        file << licensePlate << " " << exitDateTime << " " << duration << " " << charges << endl;
        file.close();
    }

    // Function to save vehicle info in txt file when vehicle is entered into the parking lot
    void saveParkingLotToFile(const string &filename) {
        ofstream file(filename);

        for (int floor = 0; floor < floors; floor++) {
            file << "Floor " << floor << ":" << endl;
            for (int spot = 0; spot < numberOfParking; spot++) {
                file << "  Spot " << spot << ": ";
                if (parkingLot[floor][spot] == nullptr) {
                    file << "Empty" << endl;
                } else {
                    file << "Vehicle " << parkingLot[floor][spot]->getLicensePlate()
                         << " type " << (dynamic_cast<Car *>(parkingLot[floor][spot]) ? "Car" : "Bike")
                         << " entered at " << parkingLot[floor][spot]->getEntryTime() << endl;
                }
            }
        }
        file.close();
    }

    // Function to load the parking lot txt file when the main function is executed
    void loadParkingLotFromFile(const string &filename) {
        ifstream file(filename);
        if (!file.is_open()) return;

        string line;
        int floor = -1, spot = -1;

        while (getline(file, line)) {
            if (line.find("Floor") != string::npos) {
                floor++;
                spot = -1;
            } else if (line.find("Spot") != string::npos) {
                spot++;
                size_t pos = line.find("Vehicle ");
                if (pos != string::npos) {
                    size_t typePos = line.find(" type ") + 6;
                    size_t timePos = line.find(" entered at ");
                    string license = line.substr(pos + 8, typePos - (pos + 8) - 6);
                    string type = line.substr(typePos, timePos - typePos);
                    string time = line.substr(timePos + 12);

                    if (type == "Car") {
                        parkingLot[floor][spot] = new Car(license, time);
                    } else if (type == "Bike") {
                        parkingLot[floor][spot] = new Bike(license, time);
                    }
                }
            }
        }
        file.close();
    }

};
// ------------------------- MAIN FUNCTION -------------------------- //
int main()
{
    Parking parking(2, 6);
    
    parking.displayParkingLot();

    return 0;
}
