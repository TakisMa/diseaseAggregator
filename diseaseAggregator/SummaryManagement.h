#ifndef DISEASEAGGREGATOR_SUMMARYMANAGEMENT_H
#define DISEASEAGGREGATOR_SUMMARYMANAGEMENT_H

#include <iostream>

using namespace std;

class WList {
private:
    string country;
    int fd;
    int fd2;
    WList* next;

public:
    int writeFD(string country);

    int readFD(string country);

    WList(string country, int fd, int fd2, WList *next);

    ~WList();
};

class WBucket {
private:
    WList* head;

public:
    int writeFD(string country);

    int readFD(string country);

    void insertSummary(string country, int fd, int fd2);

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

    void insertSummary(char* c, int fd, int fd2) {
        string country(c);
        int pos = hashS(country);
        if(!table[pos]) table[pos] = new WBucket();
        table[pos]->insertSummary(country, fd, fd2);
    }

    WHashtable(int bucketsNum);

    ~WHashtable();
};


#endif //DISEASEAGGREGATOR_SUMMARYMANAGEMENT_H
