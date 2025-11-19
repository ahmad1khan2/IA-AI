#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>



using namespace std;

// --- Global arrays and counters  ---
string userInput[50], botReply[50];
int utteranceCount = 0;

string homeArea[50], homeSize[50];
string homeInstallments[50], homePrice[50], homeDownPayment[50];
int homeCount = 0;

string currentOptionsSize[10];
string currentOptionsPrice[10], currentOptionsDownPayment[10];
string currentOptionsMonths[10];
int currentOptionsCount = 0;


// --- HELPER FUNCTION: Converts String to Int ---
// This uses if-else to build the number
int stringToInt(string s) {
    int number = 0;
    for (int i = 0; i < s.length(); i++) {
        char c = s[i];

        if (c >= '0' && c <= '9') {
            // It's a digit.
            // 1. Multiply the current number by 10 (e.g., 12 becomes 120)
            number = number * 10;
            // 2. Add the new digit (e.g., '3' becomes 3, so 120 + 3 = 123)
            number = number + (c - '0');
        }
        else if (c == ',') {
            // It's a comma, do nothing and just continue to the next character.
            continue;
        }
        else {
            // It's a letter or space. Stop parsing.
            break;
        }
    }
    return number;
}


// --- Function to handle both files  ---
void fileHandling() {
    // --- Read Utterances.txt ---
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
        utteranceCount++;
    }
    file.close();

    // --- Read Home.txt ---
    ifstream homeFile("Home.txt");
    if (!homeFile) {
        cout << "Error opening Home.txt!" << endl;
        return;
    }
    string homeLine;
    if (!getline(homeFile, homeLine)) {
        cout << "Home.txt is empty or unreadable!" << endl;
        return;
    }
    homeCount = 0;
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
        parts[partIndex] = currentPart;
        if (partIndex == 4) {
            homeArea[homeCount] = parts[0];
            homeSize[homeCount] = parts[1];
            homeInstallments[homeCount] = parts[2];
            homePrice[homeCount] = parts[3];
            homeDownPayment[homeCount] = parts[4];
            homeCount++;
        }
    }
    homeFile.close();
}


// --- Function to calculate installment  ---
// It now uses our new stringToInt function
void calculateInstallment(string size, string priceStr, string downPaymentStr, string monthsStr) {

    // --- These now call our new function ---
    int price = stringToInt(priceStr);
    int downPayment = stringToInt(downPaymentStr);
    int months = stringToInt(monthsStr);

    if (months <= 0) {
        cout << "Chatbot: Invalid number of months (" << monthsStr << "). Cannot calculate." << endl;
        cout << "Chatbot: Please press A if you want to apply for another loan. Press X to exit." << endl;
        return;
    }

    int totalLoan = price - downPayment;
    int monthlyInstallment = totalLoan / months;

    // Print the plan
    cout << "Chatbot: Great! Here is your installment plan for the " << size << " property:" << endl;
    cout << "  - Total Price: " << priceStr << endl;
    cout << "  - Down Payment: " << downPaymentStr << endl;
    cout << "  - Total Loan Amount: " << totalLoan << endl;
    cout << "  - Loan Term: " << months << " months" << endl;
    cout << "  - **Your Monthly Installment: " << monthlyInstallment << "**" << endl;

    cout << "\nChatbot: Please press A if you want to apply for another loan. Press X to exit." << endl;
}


// --- Function to handle the chat (No change) ---
void mainChat() {
    cout << "Chatbot started. Type something (X to exit)" << endl;
    string input;
    string chatState = "main";
    int homeLoanAttempt = 0;

    while (true) {
        cout << "You: ";
        getline(cin, input);

        string inputForMatching = input;
        if (inputForMatching.length() == 1) {
            inputForMatching[0] = toupper(inputForMatching[0]);
        }
        else {
            for (int k = 0; k < inputForMatching.length(); k++) {
                inputForMatching[k] = tolower(inputForMatching[k]);
            }
        }

        if (inputForMatching == "X") {
            cout << "Chatbot: Goodbye!" << endl;
            break;
        }
      
        if (chatState == "main") {
            bool found = false;
            for (int i = 0; i < utteranceCount; i++) {
                string tempUser = userInput[i];
                if (tempUser.length() == 1) {
                    tempUser[0] = toupper(tempUser[0]);
                }
                else {
                    for (int j = 0; j < tempUser.length(); j++) {
                        tempUser[j] = tolower(tempUser[j]);
                    }
                }

                if (inputForMatching == tempUser) {
                    cout << "Chatbot: " << botReply[i] << endl;
                    found = true;
                    if (inputForMatching == "H") {
                        chatState = "home_loan_area";
                        homeLoanAttempt = 0;
                    }
                    break;
                }
            }
            if (!found) {
                for (int j = 0; j < utteranceCount; j++) {
                    if (userInput[j] == "*") {
                        cout << "Chatbot: " << botReply[j] << endl;
                        found = true;
                        break;
                    }
                }
                if (!found)
                    cout << "Chatbot: Sorry, I didn't understand that." << endl;
            }
        }
        else if (chatState == "home_loan_area") {
            bool isValidOption = (inputForMatching == "1" || inputForMatching == "2" || inputForMatching == "3" || inputForMatching == "4");

            if (!isValidOption) {
                cout << "Chatbot: That is not a valid option. Please select from 1, 2, 3, or 4. Press X to exit." << endl;
            }
            else {
                string targetArea = "Area " + inputForMatching;
                bool foundOptions = false;
                currentOptionsCount = 0;

                for (int i = 0; i < homeCount; i++) {
                    if (homeArea[i] == targetArea) {
                        if (!foundOptions) {
                            cout << "Chatbot: Here are the options for " << targetArea << ":" << endl;
                        }

                        currentOptionsSize[currentOptionsCount] = homeSize[i];
                        currentOptionsPrice[currentOptionsCount] = homePrice[i];
                        currentOptionsDownPayment[currentOptionsCount] = homeDownPayment[i];
                        currentOptionsMonths[currentOptionsCount] = homeInstallments[i];

                        cout << "  " << (currentOptionsCount + 1) << ". Size: " << homeSize[i]
                            << ", Price: " << homePrice[i]
                            << ", Down Payment: " << homeDownPayment[i]
                            << " (for " << homeInstallments[i] << " months)" << endl;

                        foundOptions = true;
                        currentOptionsCount++;
                    }
                }

                if (foundOptions) {
                    cout << "\nChatbot: Please select an option (e.g., 1, 2) to see the detailed plan." << endl;
                    cout << "Chatbot: Press B to go back to the main menu." << endl;
                    chatState = "home_loan_select";
                }
                else {
                    homeLoanAttempt++;
                    if (homeLoanAttempt == 1) {
                        string availableAreas = "";
                        bool has1 = false, has2 = false, has3 = false, has4 = false;
                        for (int i = 0; i < homeCount; i++) {
                            if (homeArea[i] == "Area 1") has1 = true;
                            if (homeArea[i] == "Area 2") has2 = true;
                            if (homeArea[i] == "Area 3") has3 = true;
                            if (homeArea[i] == "Area 4") has4 = true;
                        }
                        if (has1) availableAreas += "1, ";
                        if (has2) availableAreas += "2, ";
                        if (has3) availableAreas += "3, ";
                        if (has4) availableAreas += "4, ";

                        if (availableAreas.length() > 0) {
                            availableAreas = availableAreas.substr(0, availableAreas.length() - 2);
                            cout << "Chatbot: Sorry, " << targetArea << " is not available. "
                                << "Available options are: " << availableAreas << ". "
                                << "Please select from these options or press X to exit." << endl;
                        }
                        else {
                            cout << "Chatbot: Sorry, it looks like no home loan options are available at this time. "
                                << "Please press A if you want to apply for another loan. Press X to exit." << endl;
                            chatState = "main";
                        }
                    }
                    else {
                        cout << "Chatbot: Sorry, that option is still not available. "
                            << "Please press A if you want to apply for another loan. Press X to exit." << endl;
                        chatState = "main";
                    }
                }
            }
        }
        else if (chatState == "home_loan_select") {
            if (inputForMatching == "B") {
                cout << "Chatbot: Returning to main menu. "
                    << "Please press A if you want to apply for a loan. Press X to exit." << endl;
                chatState = "main";
            }
            else {
                int selectedIndex = -1;
                try {
                    selectedIndex = stoi(inputForMatching) - 1;
                }
                catch (...) {}

                if (selectedIndex >= 0 && selectedIndex < currentOptionsCount) {
                    // Helper function to remove commas and convert to integer
                    auto parseFormattedNumber = [](const string& str) -> int {
                        string cleanStr = "";
                        for (char c : str) {
                            if (c >= '0' && c <= '9') {
                                cleanStr += c;
                            }
                        }
                        try {
                            return stoi(cleanStr);
                        }
                        catch (...) {
                            return 0;
                        }
                        };

                    // Get loan details and parse formatted numbers
                    string size = currentOptionsSize[selectedIndex];
                    int price = parseFormattedNumber(currentOptionsPrice[selectedIndex]);
                    int downPayment = parseFormattedNumber(currentOptionsDownPayment[selectedIndex]);
                    int months = stoi(currentOptionsMonths[selectedIndex]);

                    // Calculate monthly installment
                    double monthlyInstallment = (price - downPayment) / (double)months;

                    // Print complete tabular loan installment plan
                    cout << "\n" << string(70, '=') << endl;
                    cout << "COMPREHENSIVE LOAN INSTALLMENT PLAN" << endl;
                    cout << string(70, '=') << endl;
                    cout << "Property Size: " << size << endl;
                    cout << "Total Price: $" << price << endl;
                    cout << "Down Payment: $" << downPayment << endl;
                    cout << "Loan Amount: $" << (price - downPayment) << endl;
                    cout << "Loan Term: " << months << " months" << endl;
                    cout << "Monthly Installment: $" << fixed << setprecision(2) << monthlyInstallment << endl;
                    cout << string(70, '-') << endl;

                    // Table header
                    cout << setw(8) << "Month"
                        << setw(15) << "Installment"
                        << setw(20) << "Principal Paid"
                        << setw(20) << "Remaining Balance" << endl;
                    cout << string(70, '-') << endl;

                    // Calculate and display each month's details
                    double remainingBalance = price - downPayment;
                    double totalPaid = downPayment;

                    for (int month = 1; month <= months; month++) {
                        double principalThisMonth = monthlyInstallment;
                        remainingBalance -= principalThisMonth;
                        totalPaid += principalThisMonth;

                        // Handle rounding in the last month
                        if (month == months) {
                            principalThisMonth = remainingBalance + principalThisMonth;
                            remainingBalance = 0;
                        }

                        cout << setw(8) << month
                            << setw(15) << fixed << setprecision(2) << monthlyInstallment
                            << setw(20) << fixed << setprecision(2) << principalThisMonth
                            << setw(20) << fixed << setprecision(2) << (remainingBalance > 0 ? remainingBalance : 0)
                            << endl;
                    }

                    cout << string(70, '=') << endl;
                    cout << "Total Amount Paid: $" << fixed << setprecision(2) << totalPaid << endl;
                    cout << "Total Interest: $" << fixed << setprecision(2) << (totalPaid - price) << endl;
                    cout << string(70, '=') << endl;

                    cout << "\nChatbot: This completes your loan installment plan. ";
                    cout << "Press A if you want to apply for another loan. Press X to exit." << endl;
                    chatState = "main";
                }
                else {
                    cout << "Chatbot: That is not a valid selection. "
                        << "Please type a number from 1 to " << currentOptionsCount
                        << ", B to go back, or X to exit." << endl;
                }
            }
        }
    }
}

// --- Main ---
int main() {
    fileHandling();
    mainChat();
    return 0;
}//hello
//yo

