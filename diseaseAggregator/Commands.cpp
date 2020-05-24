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


int select_command(Hashtable *diseaseHT, Hashtable *countryHT, ID_Hashtable *idHT, Record *record, char *filepath, string w, int fd2) {
//        cout << "Enter command: ";
    DIR *dirp;
    struct dirent *entry;
    if(w.empty()) return -1;
    stringstream iss(w);
    iss >> w;
    if(w == "/exit") {
        cout << "exiting" << endl;
        return -1;
    }
    else if(w == "/searchPatientRecord") {
        Record *tmp = searchPatientRecord(w, idHT);
        if(tmp) cout << tmp->getRecordId() << " " << tmp->getFirstName() << " " << tmp->getLastName() << endl;
        else cout << "ID not found " << endl;
    }
    else if(w == "/diseaseFrequency") {
        cout << "inside diseaseFrequency" << endl;
        string virusName, date1, date2, country;
        int total = 0;
        int c = w.length();
        w = iss.str();
        w = w.substr(c, w.length());
        if(iss) iss >> virusName;
        if(iss) iss >> date1;
        if(iss) iss >> date2;
        if(iss) iss >> country;
        total = diseaseFrequency(virusName, date1, date2, country, diseaseHT, fd2);
        if(!country.empty()) cout << country << " " << total << endl;
        else cout << total << endl;

    }
    else if(w == "/listCountries") {
        listCountries(filepath);
//        if(int er = listCountries(filepath) != 0) return er;
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
        numPatientAdmissions(virusName, date1, date2, country, diseaseHT, filepath);

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
        numPatientDischarges(virusName, date1, date2, country, diseaseHT, filepath);
    }
    return 0;
}

int diseaseFrequency(string virusName, string date1, string date2, string country, Hashtable *diseaseHT, int fd2) {
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
        }
    }
    else{
        cout <<"error" << endl;
        delete exit;
        delete entryD;
    }
    if(country.empty()) total = diseaseHT->diseaseFrequency(virusName, entryD, exit);
    else total = diseaseHT->diseaseFrequencyC(virusName, entryD, exit, country);
    delete entryD;
    delete exit;
    write(fd2, &total, sizeof(int));
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

void
numPatientAdmissions(string virusName, string date1, string date2, string country, Hashtable *diseaseHT, char *filepath) {
    filepath = strtok(filepath, "/");
    DIR *dirp;
    struct dirent *entry;
    int counties_count = 0;
    Date *entry1 = new Date;
    Date *entry2 = new Date;
    if(date1.length() < 10 || date2.length() < 10) {
        delete entry2;
        delete entry1;
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
        }
    }
    else {
        cout << "Wrong dates" << endl;
        delete entry2;
        delete entry2;
    }
    if(entry1->compare(entry2) > 0) {
        cout << "ERROR: date1 > date2" << endl;
        delete entry2;
        delete entry1;
    }
    if(country.empty()) {
        int z = 0;

        dirp = opendir(filepath);
        if(!dirp) {
            cout << "Failed to open directory filepath in child process" << endl;
//            return errno;
        }
        while((entry = readdir(dirp)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) counties_count ++;
        }
        closedir(dirp);
        int pos = 0;
        string countries_array[counties_count], s;
        int total[counties_count];
        for(int z = 0; z < counties_count; z++) total[z] = 0;
        cout << "filepath: " << filepath << "countries_count: " << counties_count<<  endl;
        dirp = opendir(filepath);
        if(!dirp) {
            cout << "Failed to open directory filepath in child process" << endl;
//            return errno;
        }
        while((entry = readdir(dirp)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
                countries_array[pos] = entry->d_name;
                total[pos] = diseaseHT->numPatientAdmissions(virusName, entry1, entry2, countries_array[pos]);
                pos++;
            }
        }
        closedir(dirp);
        for(int z = 0; z < counties_count; z++) {
            if(total[z] != 0) cout << countries_array[z] << " " << total[z] << endl;
        }
    }
    else {
        int total = diseaseHT->numPatientAdmissions(virusName, entry1, entry2, country);
        cout << country << " " << total << endl;
    }
    delete entry2;
    delete entry1;
}

void
numPatientDischarges(string virusName, string date1, string date2, string country, Hashtable *diseaseHT, char *filepath) {
    DIR *dirp;
    struct dirent *entry;
    int counties_count = 0;
    Date *entry1 = new Date;
    Date *entry2 = new Date;
    if(date1.length() < 10 || date2.length() < 10) {
        delete entry2;
        delete entry1;
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
        }
    }
    else {
        cout << "Wrong dates" << endl;
        delete entry2;
        delete entry2;
    }
    if(entry1->compare(entry2) > 0) {
        cout << "ERROR: date1 > date2" << endl;
        delete entry2;
        delete entry1;
    }
    if(country.empty()) {
        dirp = opendir(filepath);
        if(!dirp) {
            cout << "Failed to open directory filepath in child process" << endl;
//            return errno;
        }
        while((entry = readdir(dirp)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) counties_count ++;
        }
        closedir(dirp);
        int pos = 0;
        string countries_array[counties_count], s;
        int total[counties_count];
        for(int z = 0; z < counties_count; z++) total[z] = 0;
        dirp = opendir(filepath);
        if(!dirp) {
            cout << "Failed to open directory filepath in child process" << endl;
//            return errno;
        }
        while((entry = readdir(dirp)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
                countries_array[pos] = entry->d_name;
                total[pos] = diseaseHT->numPatientDischarges(virusName, entry1, entry2,countries_array[pos]);
                pos++;
            }
        }
        closedir(dirp);
        for(int z = 0; z < counties_count; z++) {
            if(total[z] != 0) cout << countries_array[z] << " " << total[z] << endl;
        }
    }
    else {
        int total = diseaseHT->numPatientDischarges(virusName, entry1, entry2, country);
        cout << country << " " << total << endl;
    }
    delete entry2;
    delete entry1;
}
