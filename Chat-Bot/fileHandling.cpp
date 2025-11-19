#include "fileHandling.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept> 

using namespace std;

// Initialize static members
int Parser::utteranceCount = 0;
int Parser::homeCount = 0;
int Parser::scooterCount = 0;
int Parser::carCount = 0;

// --- Helper function implementations ---

int Parser::stoiSafe(const string& str) {
    try {
        // Remove commas and spaces from the string to handle formatted numbers
        string cleanedStr;
        for (char c : str) {
            if (c != ',' && c != ' ') {
                cleanedStr += c;
            }
        }

        // Return 0 if the string is empty after cleaning
        if (cleanedStr.empty()) {
            return 0;
        }

        return stoi(cleanedStr);
    }
    catch (const std::invalid_argument& e) {
        // Return 0 if conversion fails
        return 0;
    }
    catch (const std::out_of_range& e) {
        // Return 0 if number is too large
        return 0;
    }
    catch (...) {
        return 0;
    }
}

double Parser::stodSafe(const string& str) {
    try {
        // Remove commas from the string before conversion
        string cleanedStr;
        for (char c : str) {
            if (c != ',' && c != ' ') {
                cleanedStr += c;
            }
        }

        if (cleanedStr.empty()) return 0.0;

        return stod(cleanedStr);
    }
    catch (...) {
        return 0.0;
    }
}

bool Parser::stobSafe(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    // Remove any potential whitespace
    lowerStr.erase(remove(lowerStr.begin(), lowerStr.end(), ' '), lowerStr.end());

    // Check against common boolean string representations
    return (lowerStr == "1" || lowerStr == "true" || lowerStr == "yes");
}

// --- Function implementations ---

void Parser::readUtterances(vector<string>& userInput, vector<string>& botReply) {
    ifstream file("Utterances.txt");
    if (!file) {
        cout << "Error opening Utterances.txt!" << endl;
        return;
    }

    string line;
    utteranceCount = 0;

    while (getline(file, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        size_t delimiterPos = line.find('#');

        // If no delimiter is found, skip this line
        if (delimiterPos == string::npos) continue;

        string left = line.substr(0, delimiterPos);
        string right = line.substr(delimiterPos + 1);

        // Trim whitespace from both sides
        if (!left.empty()) {
            left.erase(0, left.find_first_not_of(" \t"));
            if (!left.empty()) left.erase(left.find_last_not_of(" \t") + 1);
        }

        if (!right.empty()) {
            right.erase(0, right.find_first_not_of(" \t"));
            if (!right.empty()) right.erase(right.find_last_not_of(" \t") + 1);
        }

        // Store parsed data into vectors
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
        if (homeLine.empty()) continue;

        string parts[5];
        int partIndex = 0;
        string currentPart = "";

        // Split line by '#' delimiter
        for (char c : homeLine) {
            if (c == '#') {
                if (partIndex < 5) {
                    // Trim whitespace and store part
                    currentPart.erase(0, currentPart.find_first_not_of(" \t"));
                    if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
                    parts[partIndex++] = currentPart;
                    currentPart = "";
                }
            }
            else {
                currentPart += c;
            }
        }

        // Process the final part
        if (!currentPart.empty() && partIndex < 5) {
            currentPart.erase(0, currentPart.find_first_not_of(" \t"));
            if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
            parts[partIndex] = currentPart;
        }

        if (partIndex == 4) {
            // Extract the numeric area from strings like "Area 1" or "Area 10"
            int area = 0;
            if (parts[0].find("Area") != string::npos) {
                size_t pos = parts[0].find("Area");
                string numberStr;

                // Iterate through characters after "Area" to find digits
                for (size_t i = pos + 4; i < parts[0].length(); i++) {
                    if (isdigit(parts[0][i])) {
                        numberStr += parts[0][i];
                    }
                    else if (!numberStr.empty()) {
                        break; // Stop if we hit non-digits after finding a number
                    }
                }
                if (!numberStr.empty()) {
                    area = stoi(numberStr);
                }
            }
            else {
                // Fallback for standard number format
                area = stoiSafe(parts[0]);
            }

            string size = parts[1];
            int installments = stoiSafe(parts[2]);
            int price = stoiSafe(parts[3]);
            int down = stoiSafe(parts[4]);

            // Validate data integrity before creating object
            if (price <= 0 || installments <= 0) {
                continue;
            }

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
    if (!getline(scooterFile, scooterLine)) {
        cout << "Scooter.txt is empty or unreadable!" << endl;
        return scooterLoans;
    }

    scooterCount = 0;
    while (getline(scooterFile, scooterLine)) {
        if (scooterLine.empty()) continue;

        string parts[8];
        int partIndex = 0;
        string currentPart = "";

        // Split line by '#' delimiter
        for (char c : scooterLine) {
            if (c == '#') {
                if (partIndex < 8) {
                    currentPart.erase(0, currentPart.find_first_not_of(" \t"));
                    if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
                    parts[partIndex++] = currentPart;
                    currentPart = "";
                }
            }
            else {
                currentPart += c;
            }
        }

        if (!currentPart.empty() && partIndex < 8) {
            currentPart.erase(0, currentPart.find_first_not_of(" \t"));
            if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
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

            // Validate that critical financial values are positive
            if (price <= 0 || installments <= 0) {
                continue;
            }

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
    if (!getline(carFile, carLine)) {
        cout << "Car.txt is empty or unreadable!" << endl;
        return carLoans;
    }

    carCount = 0;
    while (getline(carFile, carLine)) {
        if (carLine.empty()) continue;

        string parts[8];
        int partIndex = 0;
        string currentPart = "";

        // Split line by '#' delimiter
        for (char c : carLine) {
            if (c == '#') {
                if (partIndex < 8) {
                    currentPart.erase(0, currentPart.find_first_not_of(" \t"));
                    if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
                    parts[partIndex++] = currentPart;
                    currentPart = "";
                }
            }
            else {
                currentPart += c;
            }
        }

        if (!currentPart.empty() && partIndex < 8) {
            currentPart.erase(0, currentPart.find_first_not_of(" \t"));
            if (!currentPart.empty()) currentPart.erase(currentPart.find_last_not_of(" \t") + 1);
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

            // Validate that critical financial values are positive
            if (price <= 0 || installments <= 0) {
                continue;
            }

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

    // Use try-catch to prevent memory leaks if an error occurs during reading
    try {
        // Read and store home loans
        vector<homeLoan> homeLoans = readHome();
        for (auto& loan : homeLoans) {
            allLoans.push_back(new homeLoan(loan));
        }

        // Read and store car loans
        vector<carLoan> carLoans = readCar();
        for (auto& loan : carLoans) {
            allLoans.push_back(new carLoan(loan));
        }

        // Read and store scooter loans
        vector<scooterLoan> scooterLoans = readScooter();
        for (auto& loan : scooterLoans) {
            allLoans.push_back(new scooterLoan(loan));
        }
    }
    catch (...) {
        // If any exception happens, delete allocated memory and rethrow
        cleanupLoans(allLoans);
        throw;
    }

    return allLoans;
}

void Parser::cleanupLoans(vector<Loan*>& loans) {
    for (Loan* loan : loans) {
        delete loan;
    }
    loans.clear();
}