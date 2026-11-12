#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

using namespace std;

// Flight structure and variables
struct Flight {
    string departureAirport;
    string arrivalAirport;
    string departureTime;
    string arrivalTime;
    int otherData;
    bool assigned = false;  // Add an assigned flag to track assignment
};

// Aircraft structure
struct Aircraft {
    vector<Flight*> flights;
};

bool isOverlap(const Flight* a, const Flight* b) {
    return !(a->arrivalTime <= b->departureTime || b->arrivalTime <= a->departureTime);
}

vector<Flight> parseFlights(const string& filename) {
    vector<Flight> flights;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string departureAirport, arrivalAirport, departureTime, arrivalTime;
        int otherData;

        getline(ss, departureAirport, ',');
        getline(ss, arrivalAirport, ',');
        getline(ss, departureTime, ',');
        getline(ss, arrivalTime, ',');
        ss >> otherData;

        flights.push_back({ departureAirport, arrivalAirport, departureTime, arrivalTime, otherData });
    }

    return flights;
}

void assignFlights(vector<Flight>& flights, vector<Aircraft>& aircrafts) {
    for (Flight& flight : flights) {
        if (!flight.assigned) {
            bool assigned = false;

            for (Aircraft& aircraft : aircrafts) {
                bool canAssign = true;

                for (Flight* f : aircraft.flights) {
                    if (isOverlap(f, &flight)) {
                        canAssign = false;
                        break;
                    }
                }

                if (canAssign) {
                    flight.assigned = true;
                    aircraft.flights.push_back(&flight);
                    assigned = true;
                    break;
                }
            }

            if (!assigned) {
                Aircraft newAircraft;
                flight.assigned = true;
                newAircraft.flights.push_back(&flight);
                aircrafts.push_back(newAircraft);
            }
        }
    }
}

int main() {
    vector<Flight> flights = parseFlights("flights.txt");
    vector<Aircraft> aircrafts;

    assignFlights(flights, aircrafts);

    cout << "Optimal number of aircrafts: " << aircrafts.size() << endl;
    for (int i = 0; i < aircrafts.size(); i++) {
        if (!aircrafts[i].flights.empty()) {
            cout << "Aircraft " << i + 1 << ": ";
            for (Flight* flight : aircrafts[i].flights) {
                cout << flight->departureTime << "->" << flight->arrivalTime << " ";
            }
            cout << endl;
        }
    }

    // Output the flights with their assigned aircrafts
    for (int i = 0; i < aircrafts.size(); i++) {
        for (const auto& flight : aircrafts[i].flights) {
            cout << "From " << flight->departureAirport << " to " << flight->arrivalAirport
                << " departs at " << flight->departureTime << " and arrives at "
                << flight->arrivalTime << " using aircraft " << i + 1 << endl;
        }
    }

    return 0;
}




