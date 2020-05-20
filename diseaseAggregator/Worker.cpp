#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <dirent.h>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Tree.h"
#include "Bucket.h"
#include "Hashtable.h"
#include "Heap.h"
#include "RecordIDManagement.h"
#include "Functions.h"
#include "Record.h"

#define SIZE 10
#define DISEASE_NUM 10
#define COUNTRY_NUM 10
#define BUCKET_SIZE 256

int main(int argc, char* argv[]) {
    int numWorkers, bufferSize = 20;
    string filePath;
    /*if(checkArguments(argc, argv, numWorkers, bufferSize, filePath ) != 0) {
        return -10;
    }*/

    DIR *dirp, *aux_dipr;
    struct dirent *entry, *aux_entry;
    char *line = NULL;
    FILE* fp = NULL;
    char filepath[20];
//    strcpy(filepath, "input/");
    char filename[20];
    std::size_t lenght = 0;
    string w, countryS;
    char readbuf[bufferSize];
    char writebuf[3];


    Record *record = new Record();
    ID_Hashtable* idHT = new ID_Hashtable(SIZE);
    Hashtable *diseaseHT = new Hashtable(DISEASE_NUM, BUCKET_SIZE, disease);
    Hashtable *countryHT = new Hashtable(COUNTRY_NUM, BUCKET_SIZE, country);

    int file_count = 0, pos = 0, size, sent;
    string word, i, j, k;

    char myfifo[20] = "myfifo_";
    char auxfifo[20] = "auxfifo";
    sprintf(myfifo, "%s%d", myfifo, getpid());
    cout << "pipe name child process: " << myfifo << endl;
    if(mkfifo(myfifo, 0666) == -1 && errno != EEXIST) {
        cout << "Error with main mkfifo: " << errno << endl;
        return errno;
    }
    if(mkfifo(auxfifo, 0666) == -1 && errno != EEXIST) {
        cout << "Error with main auxfifo: " << errno << endl;
    }
    int fd2 = open(auxfifo, O_WRONLY);
    int fd = open(myfifo, O_RDONLY);
    while(true) {
//        strcpy(filepath, "input/");
        read(fd, &size, sizeof(int));
        cout << "read size child process: " << size << endl;
        int toread = size;
        while(toread != 0) {
            int er = read(fd, readbuf, size);
            cout << "er after read: " << er <<endl;
//            cout << "toread child process received: " << toread << endl;
            if(er == -1) {
                cout<< "read() error: " << errno << endl;
                break;
            }
            else toread -= er;
//            cout << "readbuf child process received: " << readbuf << endl;
//            break;
        }
        string countryS(readbuf);
//        strcpy(countryS, readbuf);
        sprintf(filepath, "%s", readbuf);
        if(strcmp(readbuf, "OK") == 0) break;
        else{
            cout << "filepath in child process is: " << filepath << endl;
            dirp = opendir(filepath);
            if(!dirp) {
                cout << "Failed to open directory" << endl;
//                return errno;
            }
            while ((entry = readdir(dirp)) != NULL) {
                if (entry->d_type == DT_REG && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".")) {
                    cout << "tmp: " << countryS << endl;
                    sprintf(filename, "%s/%s", filepath, entry->d_name);
                    cout << "tmp: " << countryS << endl;
                    cout << "Filename is: " << filename << endl;
                    fp = fopen(filename, "r");
                    while(getline(&line, &lenght, fp) != -1 ) {
                        if(record->initialize(line, entry->d_name, countryS)) cout << "Country = " << record->getCountry() << endl;
                        else cout << "Failed to initialize record " << endl;

                    }
                }
            }
        }
        cout << "String received in child process: " << readbuf << endl;
//        printf("String received in child process: %s\n", arr);
        sent = strlen("OK");
        cout << "sent size child process: " << sent << endl;
        int k = write(fd2, &sent, sizeof(int));
        cout << "child process write(fd2) k = " << k << endl;
        while(sent != 0) {
            sent -= write(fd2, "OK", sent);
        }
        break;

    }

    /* Closing and removing named pipes */
    if(close(fd) < 0) {
        cout << "Error on child process close(fd) with code: " << errno << endl;
    }
    if(close(fd2) < 0) {
        cout << "Error on child process close(fd2) with code: " << errno << endl;
    }
    unlink(myfifo);

    /*const char *cfilepath = filePath.c_str();
    dirp = opendir(cfilepath);
    if(!dirp) {
        cout << "Failed to open directory filepath in child process" << endl;
        return errno;
    }
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
            file_count = 0;
            strcpy(filepath, "input/");
            sprintf(filepath, "%s%s", filepath, entry->d_name);
            aux_dipr = opendir(filepath);
            if (!aux_dipr) {
                cout << "Failed to open directory to read filenames in child process" << endl;
                return errno;
            }
            while ((aux_entry = readdir(aux_dipr)) != NULL) {
                if (aux_entry->d_type == DT_REG && strcmp(aux_entry->d_name, "..") != 0 && strcmp(aux_entry->d_name, ".") != 0) file_count++;
            }
            closedir(aux_dipr);
            aux_dipr = opendir(filepath);
            struct Date *file_arr[file_count];
            pos = 0;
            while ((aux_entry = readdir(aux_dipr)) != NULL) {
                if (aux_entry->d_type == DT_REG && strcmp(aux_entry->d_name, "..") != 0 &&
                    strcmp(aux_entry->d_name, ".") != 0) {
                    stringstream date(aux_entry->d_name);
                    date >> word;
                    i = word.substr(0, 2);
                    j = word.substr(3, 2);
                    k = word.substr(6, 4);
                    if (check_int(i) && check_int(j) && check_int(k)) {
                        file_arr[pos] = new struct Date;
                        file_arr[pos]->date = aux_entry->d_name;
                        file_arr[pos]->day = stoi(i);
                        file_arr[pos]->month = stoi(j);
                        file_arr[pos]->year = stoi(k);
                        pos++;
                    } else {
                        cout << "Wrong filename. Date is not INT" << endl;
                        return -3;
                    }
                    cout << "Country: " << entry->d_name << " Date/File: " << aux_entry->d_name << endl;
                }
            }


            quickS(file_arr, 0, file_count - 1);
            for (int z = 0; z < file_count; z++) {
                sprintf(filename, "%s/%s", filepath, file_arr[z]->date.c_str());
                fp = fopen(filename, "r");
                if (!fp) cout << "errno: " << errno << endl;
                while (getline(&line, &lenght, fp) != -1) {
                    record = new Record;
                    if (!record->initialize(line, file_arr[z]->date, entry->d_name)) {
                        delete record;
                        continue;
                    }
                    if (record->getState() == "EXIT") {
                        if (!idHT->existsID(record->getRecordId())) {
                            cout << "EXIT without ENTER ID: " << record->getRecordId() << endl;
//                                return -10;
                        }
                        else {
                            Record *tmp = idHT->searchID(record->getRecordId());
                            if (tmp) {
                                cout << "Record Found. ID: " << record->getRecordId() << endl;
                                tmp->setExitDate(record->getExitDate());
                            }
                        }
                        delete record;
                    }
                    else {
                        idHT->insertID(record);
                        diseaseHT->insertHashTable(record);
                        countryHT->insertHashTable(record);
                    }
                }
                fclose(fp);
            }
        }
    }
    closedir(dirp);

    do {
        cout << "Enter command: ";
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
        else if(w == "topk-AgeRanges") {

        }

    } while(true);*/

    return 0;
}