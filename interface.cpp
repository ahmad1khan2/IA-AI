#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    ifstream file("Utterances.txt");
    string userInput[50], botReply[50];
    int count = 0;
    string line;

    if (!file) {
        cout << "Error opening file!" << endl;
        return 0;
    }

    // Read file line by line
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
        userInput[count] = left;
        botReply[count] = right;
        count++;
    }
    file.close();

    cout << "Chatbot started. Type something (X to exit)" << endl;

    string input;
    while (true) {
        cout << "You: ";
        getline(cin, input);

        // Convert input to consistent format for matching
        string inputForMatching = input;
        if (inputForMatching.length() == 1) {
            inputForMatching[0] = toupper(inputForMatching[0]); // single letter choice to uppercase
        }
        else {
            for (int k = 0; k < inputForMatching.length(); k++) {
                inputForMatching[k] = tolower(inputForMatching[k]); // words to lowercase
            }
        }

        if (inputForMatching == "X") {
            cout << "Chatbot: Goodbye!" << endl;
            break;
        }

        bool found = false;
        for (int i = 0; i < count; i++) {
            // Convert file data to same format for consistent matching
            string tempUser = userInput[i];
            if (tempUser.length() == 1) {
                tempUser[0] = toupper(tempUser[0]); // single letter to uppercase
            }
            else {
                for (int j = 0; j < tempUser.length(); j++) {
                    tempUser[j] = tolower(tempUser[j]); // words to lowercase
                }
            }

            if (inputForMatching == tempUser) {
                cout << "Chatbot: " << botReply[i] << endl;
                found = true;
                break;
            }
        }

        if (!found) {
            // find the line that starts with '*'
            for (int j = 0; j < count; j++) {
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

    return 0;
}