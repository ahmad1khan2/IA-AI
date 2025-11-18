#include "fileHandling.h"
#include <iostream>
#include <fstream>

using namespace std;

// Initialize static members
int Parser::utteranceCount = 0;
int Parser::homeCount = 0;
int Parser::scooterCount = 0;
int Parser::carCount = 0;

// Helper function implementations
int Parser::stoiSafe(const string& str) {
    try {
        return stoi(str);
    }
    catch (...) {
        return 0;
    }
}

double Parser::stodSafe(const string& str) {
    try {
        return stod(str);
    }
    catch (...) {
        return 0.0;
    }
}

bool Parser::stobSafe(const string& str) {
    return (str == "1" || str == "true" || str == "True" || str == "TRUE");
}

// Function implementations
void Parser::readUtterances(vector<string>& userInput, vector<string>& botReply) {
    ifstream file("Utterances.txt");
    if (!file) {
        cout << "Error opening Utterances.txt!" << endl;
        return;
    }

    string line;
    utteranceCount = 0;

    while (getline(file, line)) {
        int i = 0;
        string left = "", right = "";

        // Read everything until '#'
        while (i < line.length() && line[i] != '#') {
            left += line[i];
            i++;
        }

        i++; // skip '#'

        // Read everything after '#'
        while (i < line.length()) {
            right += line[i];
            i++;
        }

        // Store into vectors
        userInput.push_back(left);
        botReply.push_back(right);

        utteranceCount++;
    }

    file.close();
}

vector<homeLoan> Parser::readHome() {
    vector<homeLoan> homeLoans;
    ifstream homeFile("Home.txt");
    if (!homeFile) {
        cout << "Error opening Home.txt!" << endl;
        return homeLoans;
    }

    string homeLine;
    // Read and discard the header line
    if (!getline(homeFile, homeLine)) {
        cout << "Home.txt is empty or unreadable!" << endl;
        return homeLoans;
    }

    homeCount = 0;
    while (getline(homeFile, homeLine)) {
        string parts[5];
        int partIndex = 0;
        string currentPart = "";

        for (char c : homeLine) {
            if (c == '#') {
                if (partIndex < 5) {
                    parts[partIndex] = currentPart;
                    currentPart = "";
                    partIndex++;
                }
            }
            else {
                currentPart += c;
            }
        }
        if (partIndex < 5) {
            parts[partIndex] = currentPart;
        }

        if (partIndex == 4) {
            int area = stoiSafe(parts[0]);
            string size = parts[1];
            int installments = stoiSafe(parts[2]);
            int price = stoiSafe(parts[3]);
            int down = stoiSafe(parts[4]);

            homeLoan loan(area, size, installments, price, down);
            homeLoans.push_back(loan);
            homeCount++;
        }
    }
    homeFile.close();
    return homeLoans;
}

vector<scooterLoan> Parser::readScooter() {
    vector<scooterLoan> scooterLoans;
    ifstream scooterFile("Scooter.txt");
    if (!scooterFile) {
        cout << "Error opening Scooter.txt!" << endl;
        return scooterLoans;
    }

    string scooterLine;
    // Read and discard the header line
    if (!getline(scooterFile, scooterLine)) {
        cout << "Scooter.txt is empty or unreadable!" << endl;
        return scooterLoans;
    }

    scooterCount = 0;
    while (getline(scooterFile, scooterLine)) {
        string parts[8];
        int partIndex = 0;
        string currentPart = "";

        for (char c : scooterLine) {
            if (c == '#') {
                if (partIndex < 8) {
                    parts[partIndex] = currentPart;
                    currentPart = "";
                    partIndex++;
                }
            }
            else {
                currentPart += c;
            }
        }
        if (partIndex < 8) {
            parts[partIndex] = currentPart;
        }

        if (partIndex == 7) {
            string make = parts[0];
            string model = parts[1];
            int distance = stoiSafe(parts[2]);
            double chargingTime = stodSafe(parts[3]);
            int maxSpeed = stoiSafe(parts[4]);
            int installments = stoiSafe(parts[5]);
            int price = stoiSafe(parts[6]);
            int down = stoiSafe(parts[7]);

            scooterLoan loan(make, model, distance, chargingTime, maxSpeed,
                installments, price, down);
            scooterLoans.push_back(loan);
            scooterCount++;
        }
    }
    scooterFile.close();
    return scooterLoans;
}

vector<carLoan> Parser::readCar() {
    vector<carLoan> carLoans;
    ifstream carFile("Car.txt");
    if (!carFile) {
        cout << "Error opening Car.txt!" << endl;
        return carLoans;
    }

    string carLine;
    // Read and discard the header line
    if (!getline(carFile, carLine)) {
        cout << "Car.txt is empty or unreadable!" << endl;
        return carLoans;
    }

    carCount = 0;
    while (getline(carFile, carLine)) {
        string parts[8];
        int partIndex = 0;
        string currentPart = "";

        for (char c : carLine) {
            if (c == '#') {
                if (partIndex < 8) {
                    parts[partIndex] = currentPart;
                    currentPart = "";
                    partIndex++;
                }
            }
            else {
                currentPart += c;
            }
        }
        if (partIndex < 8) {
            parts[partIndex] = currentPart;
        }

        if (partIndex == 7) {
            string make = parts[0];
            string model = parts[1];
            int engine = stoiSafe(parts[2]);
            bool used = stobSafe(parts[3]);
            int year = stoiSafe(parts[4]);
            int installments = stoiSafe(parts[5]);
            int price = stoiSafe(parts[6]);
            int down = stoiSafe(parts[7]);

            carLoan loan(make, model, engine, used, year,
                installments, price, down);
            carLoans.push_back(loan);
            carCount++;
        }
    }
    carFile.close();
    return carLoans;
}

vector<Loan*> Parser::readAllLoans() {
    vector<Loan*> allLoans;

    // Read home loans
    vector<homeLoan> homeLoans = readHome();
    for (auto& loan : homeLoans) {
        allLoans.push_back(new homeLoan(loan));
    }

    // Read car loans
    vector<carLoan> carLoans = readCar();
    for (auto& loan : carLoans) {
        allLoans.push_back(new carLoan(loan));
    }

    // Read scooter loans
    vector<scooterLoan> scooterLoans = readScooter();
    for (auto& loan : scooterLoans) {
        allLoans.push_back(new scooterLoan(loan));
    }

    return allLoans;
}

void Parser::cleanupLoans(vector<Loan*>& loans) {
    for (Loan* loan : loans) {
        delete loan;
    }
    loans.clear();
}