#include "SummaryManagement.h"

int WList::writeFD(string country) {
    if(this->country == country) return fd;
    else if(next) return next->writeFD(country);
    else return -1;
}

int WList::readFD(string country) {
    if(this->country == country) return fd2;
    else if(next) return next->readFD(country);
    else return -1;
}

WList::WList(string country, int fd, int fd2, WList *next) {
    this->country = country;
    this->fd = fd;
    this->fd2 = fd2;
    this->next = next;
}

WList::~WList() {
    delete next;
}

int WBucket::writeFD(string country) {
    if(head) return head->writeFD(country);
}

int WBucket::readFD(string country) {
    if(head) return head->readFD(country);
}

void WBucket::insertSummary(string country, int fd, int fd2) {
    head = new WList(country, fd, fd2, head);
}

WBucket::WBucket() {
    head = NULL;
}

WBucket::~WBucket() {
    delete head;
}

int WHashtable::writeFD(string country) {
    int pos = hashS(country);
    if(table[pos]) return table[pos]->writeFD(country);
}

int WHashtable::readFD(string country) {
    int pos = hashS(country);
    if(table[pos]) return table[pos]->readFD(country);
}

WHashtable::WHashtable(int bucketsNum) {
    this->bucketsNum = bucketsNum;
    table = new WBucket*[bucketsNum];
    for(int i = 0; i < bucketsNum; i++) {
        table[i] = nullptr;
    }
}

WHashtable::~WHashtable() {
    for(int i = 0; i < bucketsNum; i++) {
        delete table[i];
    }
    delete []table;
}