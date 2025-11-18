#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include <string>
#include <vector>
#include "loan.h"

using namespace std;

class Parser {
private:
    // Helper functions declarations
    static int stoiSafe(const string& str);
    static double stodSafe(const string& str);
    static bool stobSafe(const string& str);

public:
    // Static data members
    static int utteranceCount;
    static int homeCount;
    static int scooterCount;
    static int carCount;

    // Function declarations
    static void readUtterances(vector<string>& userInput, vector<string>& botReply);
    static vector<homeLoan> readHome();
    static vector<scooterLoan> readScooter();
    static vector<carLoan> readCar();
    static vector<Loan*> readAllLoans();

    // Cleanup function for dynamically allocated loans
    static void cleanupLoans(vector<Loan*>& loans);
};

#endif // FILEHANDLING_H