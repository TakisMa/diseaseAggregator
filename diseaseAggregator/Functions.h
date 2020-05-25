#ifndef DISEASEAGGREGATOR_FUNCTIONS_H
#define DISEASEAGGREGATOR_FUNCTIONS_H

#include "Record.h"
#include "Hashtable.h"
#include "RecordIDManagement.h"

using namespace std;

bool check_int(string );
int checkArguments(int argc, char** argv, int &workersNum, int &bufferSize, string &path);
void swapD(struct Date *a, struct Date *b);
int partition(struct Date *array[], int low, int high);
void quickS(struct Date *array[], int low, int high);

int read_line(int fd, char *&readbuf, int bufferSize);
int read_line(int fd, char *&readbuf, int size, int bufferSize);
void write_line(int fd, char *&writebuf, int bufferSize, char *message);
int initialize_record(char *filepath, char *country, Hashtable *diseaseHT, Hashtable *countryHT, ID_Hashtable *idHT, int fd2, int bufferSize);
int sort_files(char* filepath, Date **&file_array);
char *create_fifo(char *fifo_name, pid_t childpid);
/*int commands() {
    if(!getline(cin, w)) {
        cout << "exiting" << endl;
        break;
    }
    stringstream iss(w);
    iss >> w;
    if(w.empty()) continue;
    else if(w == "/exit") {
        cout << "exiting" << endl;
        break;
    }
    else if(w == "/searchPatientRecord") {
        int id;
        iss >> w;
        id = stoi(w);
        Record *tmp = idHT->searchID(id);
        if(tmp) cout << tmp->getRecordId() << " " << tmp->getFirstName() << " " << tmp->getLastName() << endl;
        else cout << "ID not found " << endl;
    }x
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
        Date *entryD = new Date;
        Date *exit = new Date;
        if(date1.length() < 10 || date2.length() < 10) {
            delete exit;
            delete entry;
            continue;
        }
        i=date1.substr(0, 2);
        j=date1.substr(3, 2);
        k=date1.substr(6,4);
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
                continue;
            }
        }
        else{
            cout <<"error" << endl;
            delete exit;
            delete entryD;
            continue;
        }
        if(record->getEntryDate()->compare(record->getExitDate()) > 0) {
            cout <<"error" << endl;
            delete exit;
            delete entryD;
            continue;
        }
        if(country.empty()) total = diseaseHT->diseaseFrequency(virusName, entryD, exit);
        else total = diseaseHT->diseaseFrequencyC(virusName, entryD, exit, country);
        cout << total << endl;
        delete entryD;
        delete exit;
    }
    else if(w == "/listCountries") {
        dirp = opendir(cfilepath);
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
    }
    else if(w == "/numPatientAdmissions") {
        string virusName, date1, date2, country;
        int counties_count = 0;
        int c = w.length();
        w = iss.str();
        w = w.substr(c, w.length());
        if(iss) iss >> virusName;
        if(iss) iss >> date1;
        if(iss) iss >> date2;
        if(iss) iss >> country;
        Date *entry1 = new Date;
        Date *entry2 = new Date;
        if(date1.length() < 10 || date2.length() < 10) {
            delete entry2;
            delete entry1;
            continue;
        }
        i=date1.substr(0, 2);
        j=date1.substr(3, 2);
        k=date1.substr(6,4);
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
                continue;
            }
        }
        else {
            cout << "Wrong dates" << endl;
            delete entry2;
            delete entry2;
            continue;
        }
        if(entry1->compare(entry2) > 0) {
            cout << "ERROR: date1 > date2" << endl;
            delete entry2;
            delete entry1;
            continue;
        }
        if(country.empty()) {
            dirp = opendir(cfilepath);
            if(!dirp) {
                cout << "Failed to open directory filepath in child process" << endl;
                return errno;
            }
            while((entry = readdir(dirp)) != NULL) {
                if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) counties_count ++;
            }
            closedir(dirp);
            pos = 0;
            string countries_array[counties_count], s;
            int total[counties_count];
            for(int z = 0; z < counties_count; z++) total[z] = 0;
            dirp = opendir(cfilepath);
            if(!dirp) {
                cout << "Failed to open directory filepath in child process" << endl;
                return errno;
            }
            while((entry = readdir(dirp)) != NULL) {
                if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
                    countries_array[pos] = entry->d_name;
                    total[pos] = diseaseHT->numPatientAdmissions(virusName, entry1, entry2,countries_array[pos]);
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
    else if(w == "/numPatientDischarges") {
        string virusName, date1, date2, country;
        int counties_count = 0;
        int c = w.length();
        w = iss.str();
        w = w.substr(c, w.length());
        if(iss) iss >> virusName;
        if(iss) iss >> date1;
        if(iss) iss >> date2;
        if(iss) iss >> country;
        Date *entry1 = new Date;
        Date *entry2 = new Date;
        if(date1.length() < 10 || date2.length() < 10) {
            delete entry2;
            delete entry1;
            continue;
        }
        i=date1.substr(0, 2);
        j=date1.substr(3, 2);
        k=date1.substr(6,4);
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
                continue;
            }
        }
        else {
            cout << "Wrong dates" << endl;
            delete entry2;
            delete entry2;
            continue;
        }
        if(entry1->compare(entry2) > 0) {
            cout << "ERROR: date1 > date2" << endl;
            delete entry2;
            delete entry1;
            continue;
        }
        if(country.empty()) {
            dirp = opendir(cfilepath);
            if(!dirp) {
                cout << "Failed to open directory filepath in child process" << endl;
                return errno;
            }
            while((entry = readdir(dirp)) != NULL) {
                if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) counties_count ++;
            }
            closedir(dirp);
            pos = 0;
            string countries_array[counties_count], s;
            int total[counties_count];
            for(int z = 0; z < counties_count; z++) total[z] = 0;
            dirp = opendir(cfilepath);
            if(!dirp) {
                cout << "Failed to open directory filepath in child process" << endl;
                return errno;
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
}*/



static int compare_dates(const void *p, const void *q);

#endif //DISEASEAGGREGATOR_FUNCTIONS_H
