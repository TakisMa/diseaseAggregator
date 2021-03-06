#include <string>
#include "Functions.h"
#include "Record.h"
#include "SummaryList.h"
#include <iostream>
#include <cstring>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sstream>

using namespace std;

int checkArguments(int argc, char **argv, int &workerNum, int &bufferSize, string &path) {
    bool arP = false;
    bool arW = false;
    bool arB = false;

    if(argc != 7) {
        cout << "Wrong arguments!" << endl;
        return -1;
    }
    else {
        for(int i = 1; i < argc; i += 2 ) {
            if((strcmp("-i", argv[i]) == 0) && !arP){
                path = string(argv[i+1]);
                arP = true;
            }
            else if((strcmp("-w", argv[i]) == 0) && !arW){
                if(i + 1 < 9){
                    if(!check_int(argv[i+1])) {
                        cout << "Argument after -h1 is not an INT!" << endl;
                        return -3;
                    }
                    else {
                        workerNum = atoi(argv[i+1]);
                        arW = true;
                    }
                }
                else {
                    cout << "Wrong arguments!" << endl;
                    return -1;
                }
            }
            else if((strcmp("-b", argv[i]) == 0) && !arB) {
                if(i + 1 < 9){
                    if(!check_int(argv[i+1])) {
                        cout << "Argument after -b is not an INT!" << endl;
                        return -3;
                    }
                    else {
                        bufferSize = atoi(argv[i+1]);
                        arB = true;
                    }
                }
                else {
                    cout << "Wrong arguments!" << endl;
                    return -1;
                }
            }
        }
        if(!arB || !arW || !arB) {
            cout << "Wrong arguments!" << endl;
            return -1;
        }
        else if(bufferSize == 0 || workerNum == 0){
            cout << "Wrong Hashtable parameters " << endl;
            return -2;
        }
    }
    return 0;
}

bool check_int(string num) { //checks if num is a number
    int i=0;
    do{
        if(num[i]<'0' || num[i]>'9') return false;
        i++;
    }while(num[i]!='\0');
    return true;
}

void swapD(struct Date *&a, struct Date *&b) {
    Date *tmp = a;
    a = b;
    b = tmp;
}


int partition(struct Date **array, int low, int high) {
    struct Date *pivot = array[high];
    int i = low - 1;
    for(int j = low; j <= high - 1; j++) {
        if(array[j]->compare(pivot) == -1) {
            i++;
            swapD(array[i], array[j]);
        }
    }
    swapD(array[i + 1], array[high]);
    return (i+1);
}

void quickS(struct Date **array, int low, int high) {
    int pi;
    if(low < high) {
        pi = partition(array, low, high);
        quickS(array, low, pi - 1);
        quickS(array, pi+1, high);
    }
}

/* Read from fd and copy to readbuf. First message is no bytes to read(stored in size) */
int read_line(int fd, char *&readbuf, int bufferSize) {
    int size = 0;
    read(fd, &size, sizeof(int));
    readbuf = new char[size+1];
    int toread = 0;
    while (toread < size) {
        if(size-toread <= bufferSize) toread += read(fd, readbuf + toread, size-toread);
        else toread += read(fd, readbuf + toread, bufferSize);
    }
    readbuf[size] = '\0';
    return 0;
}

int read_line(int fd, char *&readbuf, int size, int bufferSize) {
    readbuf = new char[size+1];
    int toread = 0;
    while (toread < size) {
        if(size-toread <= bufferSize) toread += read(fd, readbuf + toread, size-toread);
        else toread += read(fd, readbuf + toread, bufferSize);
    }
    readbuf[size] = '\0';
    return 0;
}

void write_line(int fd, char *&writebuf, int bufferSize, char *message) {
    int size = strlen(message);
    writebuf = new char[size+1];
    strcpy(writebuf, message);
    writebuf[size] = '\0';
    write(fd, &size, sizeof(int));
    int tosend = 0;
    while(tosend < size) {
        if(size-tosend <= bufferSize) tosend += write(fd, writebuf + tosend, size-tosend);
        else tosend += write(fd, writebuf + tosend, bufferSize);
    }
}

int initialize_record(char *filepath, char *countryS, Hashtable *diseaseHT, Hashtable *countryHT, ID_Hashtable *idHT, int fd2, int bufferSize) {
    char *filename;
    char *line = NULL;
    char *writebuf;
    std::size_t lenght = 0;
    FILE *fp;
    struct Date **file_array;
    int file_count = 0;
    Record *record;
    List *list_head = new List();
    string country(countryS);
    string summary;

    file_count = sort_files(filepath, file_array);

    if(file_count < 0) return -1;
    for (int z = 0; z < file_count; z++) {
        filename = new char[strlen(filepath)+file_array[z]->date.length()+2];
        sprintf(filename, "%s/%s", filepath, file_array[z]->date.c_str());
        if(countryHT->findNewFile(file_array[z], countryS)) {
            delete [] filename;
            continue;
        }
        fp = fopen(filename, "r");
        if (!fp) {
            cout << "error with opening file errno is: " << errno << endl;
            return -2;
        }
        summary = summary + file_array[z]->date + "?" + country + "?";
//        else summary = summary + file_array[z]->date + "?";
        while (getline(&line, &lenght, fp) != -1) {
            record = new Record();
            if (!record->initialize(line, file_array[z]->date.c_str(), countryS)) {
                cout << "Failed to initialize record " << endl;
                return -3;
            }
            if(idHT->existsID(record->getRecordId()) && record->getState() == "EXIT") {
                Record *tmp = idHT->searchID(record->getRecordId());
                tmp->setExitDate(record->getExitDate());
                tmp->setState("EXIT");
                delete record;
            }
            else if(!idHT->existsID(record->getRecordId()) && record->getState() == "EXIT") {
                /*cout << "ERROR" << endl;
                cout << line << endl;*/
                delete record;
//                return -3;
            }
            else{
                idHT->insertID(record);
                diseaseHT->insertHashTable(record);
                fflush(stdout);
                countryHT->insertHashTable(record);
                list_head = list_head->insertList(record->getCountry(), record->getDiseaseId(), record);

            }
        }
        summary += list_head->getAges(countryS);
        fclose(fp);
    }
//    cout << "summary: " << summary << endl;
    fflush(stdout);
    char *s = new char[summary.length() + 1];
    strcpy(s, summary.c_str());
    write_line(fd2, writebuf, bufferSize, s);
    delete [] writebuf;
    delete [] s;
    return 1;
}
int sort_files(char *filepath, Date **&file_array) {
        DIR *dirp;
        struct dirent *entry;
        int file_count = 0, pos = 0;
        string word, i, j, k;


        dirp = opendir(filepath);
        if (!dirp) {
            cout << "Failed to open directory" << endl;
            return -1;
        }
        while ((entry = readdir(dirp)) != NULL) {
            if (entry->d_type == DT_REG && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".")) {
                file_count++;
            }
        }
        closedir(dirp);

        file_array = new Date* [file_count];
        dirp = opendir(filepath);
        if (!dirp) {
            cout << "Failed to open directory" << endl;
            return -1;
        }
        while ((entry = readdir(dirp)) != NULL) {
            if (entry->d_type == DT_REG && strcmp(entry->d_name, "..") != 0 &&
                strcmp(entry->d_name, ".") != 0) {
                stringstream date(entry->d_name);
                date >> word;
                i = word.substr(0, 2);
                j = word.substr(3, 2);
                k = word.substr(6, 4);
                if (check_int(i) && check_int(j) && check_int(k)) {
                    file_array[pos] = new struct Date;
                    file_array[pos]->date = entry->d_name;
                    file_array[pos]->day = stoi(i);
                    file_array[pos]->month = stoi(j);
                    file_array[pos]->year = stoi(k);
                    pos++;
                } else {
                    cout << "Wrong filename. Date is not INT" << endl;
                    return -3;
                }
            }
        }
        quickS(file_array, 0, file_count - 1);
        return file_count;
}
char *create_fifo(char *fifo_name, pid_t childpid) {
    char *name = new char[strlen(fifo_name) + 1];
    strcpy(name, fifo_name);
    name[strlen(fifo_name)] = '\0';
    int digits = 0, total = childpid;
    while(total != 0) {
        total /= 10;
        digits++;
    }
    char *myfifo = new char[digits + 2 + strlen(name)];
    sprintf(myfifo, "%s_%d", name, childpid);
    delete[] name;
    return myfifo;
}

/* η συνάρτηση δέχεται σαν όρισμα ολόκληρο το στρινγκ που διαβάζεται απο το named pipe και
 * αποκωδικοποιει το μηνυμα που εχει σταλει και περιεχει τα summary statistics για τον φακελο που διαβάστηκε απο τον worker.
 * Πιο αναλυτικα, μετα τον χαρακτήρα '%' ακολουθει ημερομηνια/ονομα αρχειαυ
 *      μετα τον χαρακτηρα '\' ακολουθουν ages ranges σε αύξουσα σειρά
 *      Πάντα η πρώτη λέξη που λαμβάνεται είναι η χώρα την οποια διαχειρίζομαι αρα δεν υπάρχει καποιο αναγνωριστικό γι'αυτήν.
 *      Όλες οι υπολοιπες λέξεις διαχωρίζονται με τον χαρακτήρα '?'*/
void print_summary(char *sum) {
    char *summary;
    summary = strtok(sum, "?");
    while(summary) {
        if(summary[0] == '\\'){
            summary++;
            cout << "Age range 0-20 years: " << summary << " cases" << endl;
            summary = strtok(NULL, "?");
            cout << "Age range 21-40 years: " << summary << " cases" << endl;
            summary = strtok(NULL, "?");
            cout << "Age range 41-60 years: " << summary << " cases" << endl;
            summary = strtok(NULL, "?");
            cout << "Age range 60+ years: " << summary << " cases" << endl;
            cout << endl;
        }
        else cout << summary << endl;
        summary = strtok(NULL, "?");
    }
}

int findDigits(int number) {
    int digits = 0;
    while(number != 0) {
        digits ++;
        number /= 10;
    }
    return digits;
}

int countCountries(string allCountries) {
    istringstream iss(allCountries);
    string token;
    int countries_count = 0;
    while(getline(iss, token, '?')) countries_count++;
    return countries_count;
}

void sendEntry(Record *record, int fd2, int bufferSize) {
    char *entry, *writebuf;
    int digits = findDigits(record->getRecordId());
    digits += findDigits(record->getAge());
    entry = new char[digits+record->getFirstName().length()+record->getLastName().length()+record->getDiseaseId().length()+27];
    sprintf(entry, "%d %s %s %s %d-%d-%d %d-%d-%d %d",
            record->getRecordId(),
            record->getFirstName().c_str(),
            record->getLastName().c_str(),
            record->getDiseaseId().c_str(),
            record->getEntryDate()->day, record->getEntryDate()->month, record->getEntryDate()->year,
            record->getExitDate()->day, record->getExitDate()->month, record->getExitDate()->year,
            record->getAge());
    write_line(fd2,writebuf, bufferSize, entry);
}
