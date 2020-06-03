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

WList::WList(string country, int fd, int fd2, WList *next, pid_t childpid) {
    this->country = country;
    this->fd = fd;
    this->fd2 = fd2;
    this->childpid = childpid;
    this->next = next;
}

WList::~WList() {
    delete next;
}

int WBucket::writeFD(string country) {
    if(head) return head->writeFD(country);
    else return 0;
}

int WBucket::readFD(string country) {
    if(head) return head->readFD(country);
    else return 0;
}

void WBucket::insertSummary(string country, int fd, int fd2, pid_t childpid) {
    WList *tmp = NULL;
    if(head) tmp = head->searchList(country);
    if(tmp) tmp->replaceList(fd, fd2, childpid);
    else head = new WList(country, fd, fd2, head, childpid);
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
    else return 0;
}

int WHashtable::readFD(string country) {
    int pos = hashS(country);
    if(table[pos]) return table[pos]->readFD(country);
    else return 0;

}


void WHashtable::insertSummary(char *c, int fd, int fd2, pid_t childpid) {
    string country(c);
    int pos = hashS(country);
    if(!table[pos]) table[pos] = new WBucket();
    table[pos]->insertSummary(country, fd, fd2, childpid);
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
