#ifndef FILEHANDLING_H
#define FILEHANDLING_H

#include <string>
#include <vector>
#include "applicant.h"
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
    static int personalCount;

    // Function declarations
    static void readUtterances(vector<string>& userInput, vector<string>& botReply);
    static vector<homeLoan> readHome();
    static vector<scooterLoan> readScooter();
    static vector<carLoan> readCar();
    static vector<personalLoan> readPersonal();
    static vector<Loan*> readAllLoans();

    // Cleanup function for dynamically allocated loans
    static void cleanupLoans(vector<Loan*>& loans);

    // file handling functions moved from applicant and cnic
    static bool saveApplicationToFile(const string& fileData);
    static string generateApplicationId();
    static void countApplicationsByCNIC(const string& cnic);
    static bool isValidCNIC(const string& cnic);
    static bool loadApplication(const string& appId, const string& cnic, Applicant& applicant); // for c1,c2,c3
};

#endif // FILEHANDLING_H