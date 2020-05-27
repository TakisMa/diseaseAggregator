#ifndef DISEASEAGGREGATOR_SUMMARYMANAGEMENT_H
#define DISEASEAGGREGATOR_SUMMARYMANAGEMENT_H

#include <iostream>

using namespace std;

class WList {
private:
    string country;
    int fd;
    int fd2;
    pid_t childpid;
    WList* next;

public:
    int writeFD(string country);

    int readFD(string country);

    string getAllCountries(string countries, pid_t dead_pid) {
        if(childpid == dead_pid) countries = country + "?";
        if(next) countries += next->getAllCountries(countries, dead_pid);
        return countries;
    }

    void printAll() {
        cout << "country: " << country << endl;
        if(next) next->printAll();
    }

    WList(string country, int fd, int fd2, WList *next, pid_t childpid);

    ~WList();
};

class WBucket {
private:
    WList* head;

public:
    int writeFD(string country);

    int readFD(string country);

    void insertSummary(string country, int fd, int fd2, pid_t childpid);

    string getAllCountries(string countries, pid_t dead_pid) {
        return head->getAllCountries(countries, dead_pid);
    }

    void printAll() {
        head->printAll();
    }

    WBucket();

    ~WBucket();
};

class WHashtable {
private:
    int bucketsNum;
    WBucket **table;

    int hashS(string s) {
        int hash = 0;
        for(size_t i = 0; i < s.length(); i++)
            hash += s[i];
        return hash % bucketsNum;
    }

public:
    int writeFD(string country);

    int readFD(string country);

    void insertSummary(char* c, int fd, int fd2, pid_t childpid);

    string getAllCountries(pid_t dead_pid) {
        string countries;
        for(int i = 0; i < bucketsNum; i++) {
            if(table[i]) countries += table[i]->getAllCountries(countries, dead_pid);
        }
        return countries;
    }

    void printAll() {
        for(int i = 0; i < bucketsNum; i++) if(table[i]) {
            table[i]->printAll();
        }
    }

    WHashtable(int bucketsNum);

    ~WHashtable();
};


#endif //DISEASEAGGREGATOR_SUMMARYMANAGEMENT_H
