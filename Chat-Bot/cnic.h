#ifndef CNIC_H
#define CNIC_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Function to check if a CNIC is valid (13 digits)
bool isValidCNIC(const string& cnic);

// Function to count submitted, approved, and rejected applications for a CNIC
void countApplicationsByCNIC(const string& cnic);

// Function to query user for CNIC and call the count function
void queryApplicationsByCNIC();

#endif
