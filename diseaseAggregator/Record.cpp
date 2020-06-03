#include <string>
#include <sstream>
#include "Record.h"
#include "Functions.h"
#include <iostream>
#include <cstring>

int Record::getRecordId() const {
    return recordID;
}

const string &Record::getDiseaseId() const {
    return diseaseID;
}

const string &Record::getCountry() const {
    return country;
}

const string &Record::getState() const {
    return state;
}

Date *Record::getEntryDate() {
    return &entryDate;
}

void Record::setExitDate(Date *exitDate) {
    Record::exitDate = *exitDate;
}

bool Record::isDischarged() {
    if(exitDate.year == 0 && exitDate.month == 0 && exitDate.day == 0) return false;
    else return true;
}

bool Record::initialize(string str, string date, string cou) {
    stringstream iss(str), d(date), c(cou);
    string word;
    int num;

    iss >> word;
    if(check_int(word)) num = stoi(word);
    else return false;
    recordID = num;

    iss >> word;
    state = word;
    iss >> word;
    firstName = word;
    iss >> word;
    lastName = word;
    iss >> word;
    diseaseID = word;

    iss >> word;
    if(check_int(word)) num = stoi(word);
    else return false;
    age = num;

    c >> word;
    country = word;

    d >> word;
    string i=word.substr(0, 2);
    string j=word.substr(3, 2);
    string k=word.substr(6,4);
    if(check_int(i) && check_int(j) && check_int(k) ) {
        if(state.compare("ENTER") == 0) {
            entryDate.day = stoi(i);
            entryDate.month = stoi(j);
            entryDate.year = stoi(k);
        }
        else if(state.compare("EXIT") == 0) {
            exitDate.day = stoi(i);
            exitDate.month = stoi(j);
            exitDate.year = stoi(k);
        }
        else {
            cout << "Failed in ENTER/EXIT status" << endl;
            return false;
        }
    }
    else {
        cout << "date is: " << word << endl;
        cout << "Date is not INT " << endl;
        return false;
    }
    return true;

}

Record::Record() {
    recordID = -1;
    age = -1;
    firstName = "";
    lastName = "";
    diseaseID = "";
    country = "";
    entryDate.day = 0;
    entryDate.month = 0;
    entryDate.year = 0;
    exitDate.day = 0;
    exitDate.month = 0;
    exitDate.year = 0;

}

int Record::getAge() const {
    return age;
}

void Record::setState(const string &state) {
    Record::state = state;
}

Record::~Record() {}

Date *Record::getExitDate() {
    return &exitDate;
}

