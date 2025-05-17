// Car Parking Management System (DSA-based)
// Language: C++
// Author: Sohaib Umar

#include <iostream>
#include <stack>
#include <queue>
#include <unordered_map>
#include <ctime>

using namespace std;

struct Car {
    string plateNumber;
    time_t entryTime;
};

class ParkingLot {
    int capacity;
    stack<Car> parkingStack;
    queue<Car> waitingQueue;
    unordered_map<string, time_t> carMap; // For fast search

public:
    ParkingLot(int cap) : capacity(cap) {}

    void parkCar(const string& plate) {
        time_t now = time(0);
        Car car = { plate, now };

        if (parkingStack.size() < capacity) {
            parkingStack.push(car);
            carMap[plate] = now;
            cout << "Car parked: " << plate << endl;
        } else {
            waitingQueue.push(car);
            cout << "Parking full. Car added to waiting queue: " << plate << endl;
        }
    }

    void removeCar(const string& plate) {
        stack<Car> tempStack;
        bool found = false;

        while (!parkingStack.empty()) {
            Car topCar = parkingStack.top();
            parkingStack.pop();
            if (topCar.plateNumber == plate) {
                found = true;
                time_t now = time(0);
                double duration = difftime(now, topCar.entryTime) / 60; // in minutes
                double fee = calculateFee(duration);
                cout << "Car removed: " << plate << ", Duration: " << duration << " minutes, Fee: Rs. " << fee << endl;
                carMap.erase(plate);
                break;
            } else {
                tempStack.push(topCar);
            }
        }

        // Restore the remaining cars
        while (!tempStack.empty()) {
            parkingStack.push(tempStack.top());
            tempStack.pop();
        }

        if (!found) {
            cout << "Car not found in the parking lot." << endl;
        }

        // Move from waiting queue to parking if space available
        if (!waitingQueue.empty() && parkingStack.size() < capacity) {
            Car nextCar = waitingQueue.front();
            waitingQueue.pop();
            parkingStack.push(nextCar);
            carMap[nextCar.plateNumber] = nextCar.entryTime;
            cout << "Car moved from queue to parking: " << nextCar.plateNumber << endl;
        }
    }

    void searchCar(const string& plate) {
        if (carMap.find(plate) != carMap.end()) {
            cout << "Car is currently parked: " << plate << endl;
        } else {
            cout << "Car not found in the system." << endl;
        }
    }

    double calculateFee(double minutes) {
        return 20 + (minutes / 30) * 10; // Rs. 20 base + Rs. 10 per 30 min
    }

    void displayStatus() {
        cout << "\n--- Current Parking Lot Status ---" << endl;
        stack<Car> temp = parkingStack;
        while (!temp.empty()) {
            cout << "Car: " << temp.top().plateNumber << endl;
            temp.pop();
        }
        cout << "Waiting Queue: ";
        queue<Car> q = waitingQueue;
        while (!q.empty()) {
            cout << q.front().plateNumber << " ";
            q.pop();
        }
        cout << endl;
    }
};

int main() {
    ParkingLot lot(3);
    int choice;
    string plate;

    do {
        cout << "\n\n1. Park Car\n2. Remove Car\n3. Search Car\n4. Display Status\n5. Exit\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter Plate Number: ";
                cin >> plate;
                lot.parkCar(plate);
                break;
            case 2:
                cout << "Enter Plate Number to Remove: ";
                cin >> plate;
                lot.removeCar(plate);
                break;
            case 3:
                cout << "Enter Plate Number to Search: ";
                cin >> plate;
                lot.searchCar(plate);
                break;
            case 4:
                lot.displayStatus();
                break;
            case 5:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    } while (choice != 5);

    return 0;
}
