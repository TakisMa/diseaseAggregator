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
    int writeFD(string country) {
        if(this->country == country) return fd;
        else if(next) return next->writeFD(country);
        else return -1;
    }

    int readFD(string country) {
        if(this->country == country) return fd2;
        else if(next) return next->readFD(country);
        else return -1;
    }

    WList(string country, int fd, int fd2, WList *next) {
        this->country = country;
        this->fd = fd;
        this->fd2 = fd2;
        this->next = next;
    }

    ~WList() {
        delete next;
    }
};

class WBucket {
private:
    WList* head;

public:
    int writeFD(string country) {
        if(head) return head->writeFD(country);
    }

    int readFD(string country) {
        if(head) return head->readFD(country);
    }

    void insertSummary(string country, int fd, int fd2) {
        head = new WList(country, fd, fd2, head);
    }

    WBucket() {
        head = NULL;
    }

    ~WBucket() {
        delete head;
    }
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
    int writeFD(string country) {
        int pos = hashS(country);
        if(table[pos]) return table[pos]->writeFD(country);
    }

    int readFD(string country) {
        int pos = hashS(country);
        if(table[pos]) return table[pos]->readFD(country);
    }

    void insertSummary(char* c, int fd, int fd2) {
        string country(c);
        int pos = hashS(country);
        if(!table[pos]) table[pos] = new WBucket();
        table[pos]->insertSummary(country, fd, fd2);
    }

    WHashtable(int bucketsNum) {
        this->bucketsNum = bucketsNum;
        table = new WBucket*[bucketsNum];
        for(int i = 0; i < bucketsNum; i++) {
            table[i] = nullptr;
        }
    }

    ~WHashtable() {
        for(int i = 0; i < bucketsNum; i++) {
            delete table[i];
        }
        delete []table;
    }
};


#endif //DISEASEAGGREGATOR_SUMMARYMANAGEMENT_H
