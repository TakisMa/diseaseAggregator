#include "Commands.h"
#include <sstream>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <unistd.h>
#include "Hashtable.h"
#include "Functions.h"
#include "RecordIDManagement.h"

Record *searchPatientRecord(string w, ID_Hashtable *idHT) {
    stringstream iss(w);
    int id;
    iss >> w;
    id = stoi(w);
    Record *tmp = idHT->searchID(id);
    if(tmp) return tmp;
    else return NULL;
}


int select_command(Hashtable *diseaseHT, Hashtable *countryHT, ID_Hashtable *idHT, char *filepath, string w, int fd2, int bufferSize) {
    if(w.empty()) return -1;
    char *writebuf;
    stringstream iss(w);
    iss >> w;
    if(w == "/exit") {
        cout << "exiting" << endl;
        return 1;
    }
    else if(w == "/searchPatientRecord") {
        iss >> w;
        Record *tmp = searchPatientRecord(w, idHT);
        if(tmp) cout << tmp->getRecordId() << " " << tmp->getFirstName() << " " << tmp->getLastName() << endl;
        else cout << "ID not found " << endl;
        return 1;
    }
    else if(w == "/diseaseFrequency") {
        string virusName, date1, date2, country;
        int total = 0;
        int c = w.length();
        w = iss.str();
        w = w.substr(c, w.length());
        if(iss) iss >> virusName;
        if(iss) iss >> date1;
        if(iss) iss >> date2;
        if(iss) iss >> country;
        total = diseaseFrequency(virusName, date1, date2, country, diseaseHT);
        if(total < 0) return -1;
        int digits = findDigits(total);
        if(!country.empty()) {
            char *message = new char[country.length()+digits+2];
            sprintf(message, "%s %d", country.c_str(), total);
            write_line(fd2, writebuf, bufferSize, message);
            delete [] message;
        }
        else {
            char *message = new char[digits+1];
            sprintf(message, "%d", total);
            message[digits] = '\0';
            write_line(fd2, writebuf, bufferSize, message);
        }
        return 1;
    }
    else if(w == "/listCountries") {
        string countries = countryHT->getCountry().c_str();
        char *c = new char[countries.length() + 1];
        strcpy(c, countries.c_str());
        c[countries.length()] = '\0';
        char *countriesC = strtok(c, "?");
        while(countriesC != NULL) {
            cout << countriesC << " " << getpid() << endl;
            countriesC = strtok(NULL, "?");
        }
        return 1;
    }
    else if(w == "/numPatientAdmissions") {
        string virusName, date1, date2, country;
        int c = w.length();
        w = iss.str();
        w = w.substr(c, w.length());
        if(iss) iss >> virusName;
        if(iss) iss >> date1;
        if(iss) iss >> date2;
        if(iss) iss >> country;
        int tmp = numPatientAdmissions(virusName, date1, date2, country, diseaseHT, countryHT, fd2, bufferSize);
        if(tmp < 0) return -1;
        return 1;
    }
    else if(w == "/numPatientDischarges") {
        string virusName, date1, date2, country;
        int c = w.length();
        w = iss.str();
        w = w.substr(c, w.length());
        if(iss) iss >> virusName;
        if(iss) iss >> date1;
        if(iss) iss >> date2;
        if(iss) iss >> country;
        return numPatientDischarges(virusName, date1, date2, country, diseaseHT, countryHT, fd2, bufferSize);
    }
    return -1;
}

int diseaseFrequency(string virusName, string date1, string date2, string country, Hashtable *diseaseHT) {
    int total = 0;
    Date *entryD = new Date;
    Date *exit = new Date;
    if(date1.length() < 10 || date2.length() < 10) {
        delete exit;
        delete entryD;
    }
    string i=date1.substr(0, 2);
    string j=date1.substr(3, 2);
    string k=date1.substr(6,4);
    if(check_int(i) && check_int(j) && check_int(k) ) {
        entryD->day = stoi(i);
        entryD->month = stoi(j);
        entryD->year = stoi(k);
        if(date2 == "-") {
            exit->day = 0;
            exit->month = 0;
            exit->year = 0;
        }
        else if(check_int(i=date2.substr(0, 2)) && check_int(j=date2.substr(3, 2)) && check_int(k=date2.substr(6,4) )) {
            exit->day = stoi(i);
            exit->month = stoi(j);
            exit->year = stoi(k);

        }
        else{
            cout <<"error" << endl;
            delete exit;
            delete entryD;
            return -1;
        }
    }
    else{
        cout <<"error" << endl;
        delete exit;
        delete entryD;
        return -1;
    }
    if(country.empty()) total = diseaseHT->diseaseFrequency(virusName, entryD, exit);
    else total = diseaseHT->diseaseFrequencyC(virusName, entryD, exit, country);
    delete entryD;
    delete exit;
    return total;
}

int listCountries(char *filepath) {
    DIR *dirp;
    struct dirent *entry;
    dirp = opendir(filepath);
    if(!dirp) {
        cout << "Failed to open directory filepath in child process" << endl;
        return errno;
    }
    while((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
            cout << entry->d_name << " " << getpid() << endl;
        }
    }
    closedir(dirp);
    return 0;
}

int numPatientAdmissions(string virusName, string date1, string date2, string country, Hashtable *diseaseHT, Hashtable *countryHT, int fd2, int bufferSize) {
    Date *entry1 = new Date;
    Date *entry2 = new Date;
    if(date1.length() < 10 || date2.length() < 10) {
        delete entry2;
        delete entry1;
        return -1;
    }
    char *writebuf;
    string i=date1.substr(0, 2);
    string j=date1.substr(3, 2);
    string k=date1.substr(6,4);
    if(check_int(i) && check_int(j) && check_int(k)) {
        entry1->day = stoi(i);
        entry1->month = stoi(j);
        entry1->year = stoi(k);
        if(check_int(i=date2.substr(0, 2)) && check_int(j=date2.substr(3, 2)) && check_int(k=date2.substr(6,4) )) {
            entry2->day = stoi(i);
            entry2->month = stoi(j);
            entry2->year = stoi(k);
        }
        else {
            cout << "Wrong dates " << endl;
            delete entry2;
            delete entry1;
            return -1;
        }
    }
    else {
        cout << "Wrong dates" << endl;
        delete entry2;
        delete entry1;
        return -1;
    }
    if(entry1->compare(entry2) > 0) {
        cout << "ERROR: date1 > date2" << endl;
        delete entry2;
        delete entry1;
        return -1;
    }
    if(country.empty()) {
        string allCountries = countryHT->getCountry();
        int countries_count = countCountries(allCountries);
        write(fd2, &countries_count, sizeof(int));
        stringstream iss(allCountries);
        string token;
        int digits = 0;
        while(getline(iss, token, '?')) {
            int total = diseaseHT->numPatientAdmissions(virusName, entry1, entry2, token);
            digits = findDigits(total);
            char *message = new char[token.length()+digits+2];
            sprintf(message, "%s %d", token.c_str(), total);
            write_line(fd2, writebuf, bufferSize, message);
        }
    }
    else {
        int total = diseaseHT->numPatientAdmissions(virusName, entry1, entry2, country);
        int digits = findDigits(total);
        char *message = new char[country.length()+digits+2];
        sprintf(message, "%s %d", country.c_str(), total);
        write_line(fd2, writebuf, bufferSize, message);
    }
    delete entry2;
    delete entry1;
    return 1;
}

int numPatientDischarges(string virusName, string date1, string date2, string country, Hashtable *diseaseHT, Hashtable *countryHT, int fd2, int bufferSize) {
    Date *entry1 = new Date;
    Date *entry2 = new Date;
    char *writebuf;
    if(date1.length() < 10 || date2.length() < 10) {
        delete entry2;
        delete entry1;
        return -1;
    }
    string i=date1.substr(0, 2);
    string j=date1.substr(3, 2);
    string k=date1.substr(6,4);
    if(check_int(i) && check_int(j) && check_int(k)) {
        entry1->day = stoi(i);
        entry1->month = stoi(j);
        entry1->year = stoi(k);
        if(check_int(i=date2.substr(0, 2)) && check_int(j=date2.substr(3, 2)) && check_int(k=date2.substr(6,4) )) {
            entry2->day = stoi(i);
            entry2->month = stoi(j);
            entry2->year = stoi(k);
        }
        else {
            cout << "Wrong dates " << endl;
            delete entry2;
            delete entry1;
            return -1;
        }
    }
    else {
        cout << "Wrong dates" << endl;
        delete entry2;
        delete entry2;
        return -1;
    }
    if(entry1->compare(entry2) > 0) {
        cout << "ERROR: date1 > date2" << endl;
        delete entry2;
        delete entry1;
        return -1;
    }
    if(country.empty()) {
        string allCountries = countryHT->getCountry();
        int countries_count = countCountries(allCountries);
        write(fd2, &countries_count, sizeof(int));
        stringstream iss(allCountries);
        string token;
        int digits = 0;
        while(getline(iss, token, '?')) {
            int total = diseaseHT->numPatientDischarges(virusName, entry1, entry2, token);
            digits = findDigits(total);
            char *message = new char[token.length()+digits+2];
            sprintf(message, "%s %d", token.c_str(), total);
            write_line(fd2, writebuf, bufferSize, message);
        }
    }
    else {
        int total = diseaseHT->numPatientDischarges(virusName, entry1, entry2, country);
        int digits = findDigits(total);
        char *message = new char[country.length()+digits+2];
        sprintf(message, "%s %d", country.c_str(), total);
        write_line(fd2, writebuf, bufferSize, message);
    }
    delete entry2;
    delete entry1;
    return 1;
}
