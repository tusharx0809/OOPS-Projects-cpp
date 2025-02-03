<h1>This is a MultiStorey Parking Lot built usign Object Oriented Programming in C++.</h1>

<h2><em>Features</em></h2>
<ul>
    <li>Vehicle is parked at the first found empty parking spot.</li>
    <li>If parking spot is not found, the vehicle moves onto the next floor.</li>
    <li>License Plate and entry date and time is recorded for the vehicle and logged into the parking lot txt file.</li>
    <li>When exiting parking charges are calcuated as Rs0.2 per minute for Car and Rs0.1 per minute for bike and logged into the charges txt file.</li>
</ul>

<h3>Classes</h3>
<ul>
    <li>
        Parking Class and its functions
        <ul>
            <li><strong>string getCurrentDateTime():</strong> Returns string of current date time in format "MM-DD-YYYY, HH:MM"</li>
            <li><strong>long int getParkingTime():</strong> Takes two arguments as entry time and exit time and returns the differnce in minutes.</li>
            <li><strong>bool ParkVehicle():</strong> Takes a pointer to Vehicle object as argument and parks at first empty spot.</li>
            <li><strong>bool removeVehicle():</strong> Takes the string licensePlate as argument and removes the car and log charges to file/.</li>           
            <li><strong>void displayParkingLot():</strong> Displays the parking Lot.</li>
            <li><strong>void logChargesToFile():</strong> Takes five arguments: License plate, exit date time, duration, charges and filename in which the charges are stored.</li>
            <li><strong>void saveParkingToFile:</strong> Takes one argument as filename in which the parking lot is stored.</li>
            <li><strong>void loadParkingLotFromFile(): Takes one argument from which the parking lot is loaded.</strong></li>
        </ul>
    </li>
    <li>Inherited class for car and bikes/scooters.</li>
    <li>Different parking charges for cars and bikes.</li>
</ul>