#include <bits/stdc++.h>
using namespace std;

class Parking
{
private:
    int floors;                                               // number of floors in multistorey parking
    int numberOfParking;                                      // number of parkings on one storey
    vector<vector<tuple<string, string, string>>> parkingLot; // 2d vector which represents multistorey parking, tuple represents license plate and time at which the customer parked the vehicle

public:
    Parking(int floors, int numberOfParking) : floors(floors), numberOfParking(numberOfParking)
    {
        parkingLot.resize(floors, vector<tuple<string, string, string>>(numberOfParking, make_tuple("", "", "")));
        loadParkingLotFromFile("parking_lot.txt");
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

        long int parkingTime = chrono::duration_cast<chrono::minutes>(tp2- tp1).count();

        return parkingTime;
    }

    // Function to calculate parking charges per minute
    double calculateParkingCharges(int minutes, const string &type)
    {
        return (type == "Car") ? minutes * 0.15 : minutes * 0.075;
    }

    // Function to add a vehicle at the first empty parking spot
    bool parkVehicle(const string &licensePlate, const string &type)
    {
        int spot = 0;
        int floor = 0;

        while (floor < floors)
        {
            if (get<0>(parkingLot[floor][spot]) == "")
            {
                string parkingDateTime = getCurrentDateTime();
                parkingLot[floor][spot] = make_tuple(licensePlate, type, parkingDateTime);
                saveParkingLotToFile("parking_lot.txt");
                cout << type << " parked at floor: " << floor << " and spot: " << spot << endl;
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
            string storedLicensePlate = get<0>(parkingLot[floor][spot]);
            if (!storedLicensePlate.empty() && storedLicensePlate == licensePlate)
            {
                string exitDateTime = getCurrentDateTime();
                long int totalParkingTime = getParkingTime(get<2>(parkingLot[floor][spot]), exitDateTime);
                double charges = calculateParkingCharges(totalParkingTime, get<1>(parkingLot[floor][spot]));

                parkingLot[floor][spot] = make_tuple("", "", "");
                cout << get<1>(parkingLot[floor][spot]) << " " << licensePlate << " exited from floor:" << floor << " and spot: " << spot << " at: " << exitDateTime << " Charges: " << charges << endl;
                saveParkingLotToFile("parking_lot.txt");
                displayParkingLot();  // Now called only once
                logChargesToFile(licensePlate, exitDateTime, totalParkingTime, charges, "charges.txt");
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
                if (get<0>(parkingLot[i][j]) == "")
                {
                    cout << "Empty";
                }
                else
                {
                    cout << "Vehicle " << get<0>(parkingLot[i][j])
                         << " type " << get<1>(parkingLot[i][j])
                         << " entered at " << get<2>(parkingLot[i][j]);
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
    void saveParkingLotToFile(const string &filename)
    {
        ofstream file(filename);

        if (!file.is_open())
        {
            cout << "Error Opening File!" << endl;
            return;
        }

        for (int i = 0; i < floors; i++)
        {
            file << "Floor " << i << ":" << endl;
            for (int j = 0; j < numberOfParking; j++)
            {
                file << "  Spot " << j << ": ";
                if (get<0>(parkingLot[i][j]) == "")
                {
                    file << "Empty" << endl;
                }
                else
                {
                    file << "Vehicle " << get<0>(parkingLot[i][j]) << " type " << get<1>(parkingLot[i][j]) << " entered at " << get<2>(parkingLot[i][j]) << endl;
                }
            }
        }
        file.close();
        cout << "Parking Lot Saved to " << filename << endl;
    }

    // Function to load the parking lot txt file when the main function is executed
    void loadParkingLotFromFile(const string &filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error Opening File!" << endl;
        return;
    }

    string line;
    int floor = -1;
    int spot = -1;

    while (getline(file, line))
    {
        // Detect the floor based on the "Floor" keyword
        if (line.find("Floor") != string::npos)
        {
            floor++;
            spot = -1;  // Reset spot index when a new floor is encountered
        }
        // Detect the spot based on the "Spot" keyword
        else if (line.find("Spot") != string::npos)
        {
            spot++;
            size_t pos = line.find("Vehicle ");
            if (pos != string::npos)
            {
                size_t time_pos = line.find(" entered at ");
                size_t type_pos = line.find(" type ") + 6; // Fix the position for type field

                string licensePlate = line.substr(pos + 8, type_pos - (pos + 8) - 6);  // Extract license plate
                string type = line.substr(type_pos, time_pos - type_pos);               // Extract vehicle type
                string time = line.substr(time_pos + 12);  // Extract time, adjusted by " entered at "

                // Check if license plate, type, or time are empty or invalid
                if (!licensePlate.empty() && !type.empty() && !time.empty())
                {
                    // Store all information in parkingLot tuple
                    parkingLot[floor][spot] = make_tuple(licensePlate, type, time);
                }
                else
                {
                    // If data is incomplete, mark as empty
                    parkingLot[floor][spot] = make_tuple("", "", "");
                }
            }
            else
            {
                // If no vehicle is found, mark the spot as empty
                parkingLot[floor][spot] = make_tuple("", "", "");
            }
        }
    }
}

};

int main()
{
    Parking parking(2, 5);

    // Park some vehicles
    parking.removeVehicle("ABC123");

    // Vehicle will be removed

    return 0;
}
