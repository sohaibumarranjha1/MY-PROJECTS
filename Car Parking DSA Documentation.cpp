// Car Parking Management System (DSA-based)
// Language: C++
// Author: Sohaib Umar

/*
==============================
PROJECT DOCUMENTATION
==============================

Project Title: Car Parking Management System
Developer: Sohaib Umar
Language: C++

==============================
OBJECTIVE:
==============================
To develop a simple car parking system using fundamental Data Structures in C++. The system efficiently manages car entries, exits, searching, and queuing with real-time parking lot status.

==============================
TOOLS & TECHNOLOGIES USED:
==============================
- Language: C++
- Compiler: g++ or any C++ compiler
- Data Structures: Stack, Queue, Hash Map (unordered_map)
- Libraries: iostream, stack, queue, unordered_map, ctime

==============================
FEATURES:
==============================
1. Park Car: Adds a car to the parking lot or waiting queue if full.
2. Remove Car: Removes a car from the lot, calculates duration and fee.
3. Search Car: Finds if a car is parked.
4. Display Status: Shows current parked cars and waiting queue.

==============================
DATA STRUCTURES USED:
==============================
- Stack: For LIFO parking system where the last car must leave first.
- Queue: For managing incoming cars when parking is full.
- Hash Map: For quick car search and tracking entry times.

==============================
FEE STRUCTURE:
==============================
- Rs. 20 base fee
- Rs. 10 per additional 30 minutes

==============================
SAMPLE OUTPUT:
==============================
1. Park Car: "Car parked: ABC123"
2. Remove Car: "Car removed: ABC123, Duration: 60 minutes, Fee: Rs. 40"
3. Full Parking: "Parking full. Car added to waiting queue: XYZ789"
4. Search: "Car is currently parked: ABC123"

==============================
CONCLUSION:
==============================
The system successfully implements core DSA concepts to mimic a real-world car parking lot. It is suitable for educational purposes to demonstrate stack, queue, and hash operations in C++.
*/

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