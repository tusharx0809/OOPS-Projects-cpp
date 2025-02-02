#include <bits/stdc++.h>
using namespace std;

class Parking
{
private:
    int floors;                                      // number of floors in multistorey parking
    int numberOfParking;                             // number of parkings on one storey
    vector<vector<pair<string, string>>> parkingLot; // 2d vector which represents multistorey parking, internal unordered map represents parking number and its value representing a vector with license plate and time entered in parking

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
    }
    string getCurrentDateTime()
    {
        time_t timestamp;
        time(&timestamp);

        // Convert the timestamp to local time
        struct tm *local_time = localtime(&timestamp);

        // Extract and print date and time
        int year = local_time->tm_year + 1900; // tm_year is years since 1900
        int month = local_time->tm_mon + 1;    // tm_mon is 0-11
        int day = local_time->tm_mday;         // tm_mday is day of the month (1-31)
        int hour = local_time->tm_hour;        // tm_hour is hour (0-23)
        int minute = local_time->tm_min;       // tm_min is minute (0-59)
        int second = local_time->tm_sec;       // tm_sec is second (0-59)

        string dateTime = to_string(month) + "-" + to_string(day) + "-" + to_string(year) + "," + to_string(hour) + ":" + to_string(minute);

        return dateTime;
    }

    bool parkVehicle(const string &licensePlate)
    {
        int spot = 0;
        int floor = 0;

        while (floor < floors)
        {
            if (parkingLot[floor][spot].first == "")
            {
                string currentDateTime = getCurrentDateTime();
                parkingLot[floor][spot] = {licensePlate, currentDateTime};
                cout << "Car Parked at floor:" << floor << " and spot: " << spot << endl;
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
        return false;
    }

    bool removeVehicle(int floor, int spot)
    {
        if (floor < 0 || floor >= floors || spot < 0 || spot >= numberOfParking)
        {
            cout << "Invalid floor or spot number." << endl;
            return false;
        }
        if (parkingLot[floor][spot].first == "")
        {
            cout << "Parking spot is already empty." << endl;
            return false;
        }
        parkingLot[floor][spot] = {"", ""};
        cout << "Vehicle removed successfully from floor " << floor << ", spot " << spot << "." << endl;
        return true;
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
};

int main()
{
    Parking parking(2, 2); // 2 floors, 3 parking spots per floor

    // Vehicle will be parked wherever it finds the first empty spot parking

    parking.parkVehicle("ABC123");
    parking.parkVehicle("DEF456");
    parking.parkVehicle("QWE345");
    parking.parkVehicle("QWT345");
    parking.removeVehicle(0, 1);
    parking.parkVehicle("ERT456");
    parking.displayParkingLot();

    return 0;
}