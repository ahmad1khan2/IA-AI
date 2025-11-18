#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include <string>    // For std::string
#include <iostream>  // For std::cout
#include <fstream>   // For std::ifstream

// Use the std namespace to match your project's style
using namespace std;

// The Parser class, with all declarations and definitions
// included directly in the header file.
class Parser {
public:
    // --- Static data members for the counts ---
    static int utteranceCount;
    static int homeCount;
    static int scooterCount; // NEW
    static int carCount;     // NEW

    // --- Static function for reading Utterances.txt ---
    static void readUtterances(string userInput[], string botReply[]) {
        ifstream file("Utterances.txt");
        if (!file) {
            cout << "Error opening Utterances.txt!" << endl;
            return;
        }
        string line;
        utteranceCount = 0; // Use the static class member
        while (getline(file, line)) {
            int i = 0;
            string left = "", right = "";
            while (i < line.length() && line[i] != '#') {
                left += line[i];
                i++;
            }
            i++; // move past #
            while (i < line.length()) {
                right += line[i];
                i++;
            }
            userInput[utteranceCount] = left;
            botReply[utteranceCount] = right;
            utteranceCount++; // Increment the static class member
        }
        file.close();
    }

    // --- Static function for reading Home.txt ---
    static void readHome(string homeArea[], string homeSize[], string homeInstallments[], string homePrice[], string homeDownPayment[]) {
        ifstream homeFile("Home.txt");
        if (!homeFile) {
            cout << "Error opening Home.txt!" << endl;
            return;
        }

        string homeLine;
        // Read and discard the header line
        if (!getline(homeFile, homeLine)) {
            cout << "Home.txt is empty or unreadable!" << endl;
            return;
        }

        // Read the data lines from Home.txt
        homeCount = 0; // Use the static class member
        while (getline(homeFile, homeLine)) {
            string parts[5];
            int partIndex = 0;
            string currentPart = "";
            int i = 0;
            while (i < homeLine.length()) {
                if (homeLine[i] == '#') {
                    parts[partIndex] = currentPart;
                    currentPart = "";
                    partIndex++;
                }
                else {
                    currentPart += homeLine[i];
                }
                i++;
            }
            parts[partIndex] = currentPart; // Add the last part

            // Assign parts to arrays if all 5 were found
            if (partIndex == 4) {
                homeArea[homeCount] = parts[0];
                homeSize[homeCount] = parts[1];
                homeInstallments[homeCount] = parts[2];
                homePrice[homeCount] = parts[3];
                homeDownPayment[homeCount] = parts[4];
                homeCount++; // Increment the static class member
            }
        }
        homeFile.close();
    }

    // --- NEW: Static function for reading Scooter.txt ---
    static void readScooter(string make[], string model[], string distance[], string chargingTime[], string maxSpeed[], string installments[], string price[], string downPayment[]) {
        ifstream scooterFile("Scooter.txt"); // Assume filename is Scooter.txt
        if (!scooterFile) {
            cout << "Error opening Scooter.txt!" << endl;
            return;
        }

        string scooterLine;
        // Read and discard the header line
        if (!getline(scooterFile, scooterLine)) {
            cout << "Scooter.txt is empty or unreadable!" << endl;
            return;
        }

        // Read the data lines from Scooter.txt
        scooterCount = 0; // Use the new static class member
        while (getline(scooterFile, scooterLine)) {
            string parts[8]; // This file has 8 columns
            int partIndex = 0;
            string currentPart = "";
            int i = 0;
            while (i < scooterLine.length()) {
                if (scooterLine[i] == '#') {
                    parts[partIndex] = currentPart;
                    currentPart = "";
                    partIndex++;
                }
                else {
                    currentPart += scooterLine[i];
                }
                i++;
            }
            parts[partIndex] = currentPart; // Add the last part

            // Assign parts to arrays if all 8 were found
            if (partIndex == 7) {
                make[scooterCount] = parts[0];
                model[scooterCount] = parts[1];
                distance[scooterCount] = parts[2];
                chargingTime[scooterCount] = parts[3];
                maxSpeed[scooterCount] = parts[4];
                installments[scooterCount] = parts[5];
                price[scooterCount] = parts[6];
                downPayment[scooterCount] = parts[7];
                scooterCount++; // Increment the static class member
            }
        }
        scooterFile.close();
    }

    // --- NEW: Static function for reading Car.txt ---
    static void readCar(string make[], string model[], string engine[], string used[], string manYear[], string installments[], string price[], string downPayment[]) {
        ifstream carFile("Car.txt"); // Assume filename is Car.txt
        if (!carFile) {
            cout << "Error opening Car.txt!" << endl;
            return;
        }

        string carLine;
        // Read and discard the header line
        if (!getline(carFile, carLine)) {
            cout << "Car.txt is empty or unreadable!" << endl;
            return;
        }

        // Read the data lines from Car.txt
        carCount = 0; // Use the new static class member
        while (getline(carFile, carLine)) {
            string parts[8]; // This file also has 8 columns
            int partIndex = 0;
            string currentPart = "";
            int i = 0;
            while (i < carLine.length()) {
                if (carLine[i] == '#') {
                    parts[partIndex] = currentPart;
                    currentPart = "";
                    partIndex++;
                }
                else {
                    currentPart += carLine[i];
                }
                i++;
            }
            parts[partIndex] = currentPart; // Add the last part

            // Assign parts to arrays if all 8 were found
            if (partIndex == 7) {
                make[carCount] = parts[0];
                model[carCount] = parts[1];
                engine[carCount] = parts[2];
                used[carCount] = parts[3];
                manYear[carCount] = parts[4];
                installments[carCount] = parts[5];
                price[carCount] = parts[6];
                downPayment[carCount] = parts[7];
                carCount++; // Increment the static class member
            }
        }
        carFile.close();
    }
};

// --- Initialize ALL static data members ---
int Parser::utteranceCount = 0;
int Parser::homeCount = 0;
int Parser::scooterCount = 0; // NEW
int Parser::carCount = 0;     // NEW

#endif // FILEHANDLING_H