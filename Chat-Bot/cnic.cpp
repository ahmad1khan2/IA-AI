#include "cnic.h"
#include "fileHandling.h"


void queryApplicationsByCNIC() {
    string cnic;

    while (true) {
        cout << "Enter CNIC to query (13 digits, without dashes): ";
        getline(cin, cnic);

        if (Parser::isValidCNIC(cnic)) {  
            break;
        }
        else {
            cout << "Error: CNIC must be 13 digits without dashes. Please try again.\n\n";
        }
    }

    Parser::countApplicationsByCNIC(cnic);  
}

