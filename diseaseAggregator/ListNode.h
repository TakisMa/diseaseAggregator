#ifndef DISEASEMONITOR_LISTNODE_H
#define DISEASEMONITOR_LISTNODE_H

#include <iostream>
#include <cstdlib>
#include "Record.h"
#include "Tree.h"

//enum type_t:bool {disease, country};

using namespace std;

class ListNode {
private:
    ListNode* next;
    Record* record;
    Tree* tree;

public:
    void topk(string country, Heap *heap, Date *entry, Date *exit, type_t type);

    void topk(string country, Heap *heap, type_t type);

    void numCurrentPatients(string disease);

    void numCurrentPatients();

    void globalDiseaseStats(Date *entry, Date *exit);

    void globalDiseaseStats();

    int numPatientAdmissions(Date *date1, Date *date2, string countries);

    int numPatientDischarges(Date *date1, Date *date2, string countries);

    int countIncidents(Date *entry, Date *exit, string country);

    int countIncidents(Date *entry, Date *exit);

    string getCountry() {
        string countries;
        countries += record->getCountry();
        countries += "?";
        if(next) countries += next->getCountry();
        return countries;
    }

    Record *getRecord() const {
        return record;
    }

    void compareCountry(Record *rec) {
        if(record->getCountry().compare(rec->getCountry()) == 0);
    }

    void compareDisease(Record *rec) {
        if(record->getDiseaseId() == rec->getDiseaseId());
    }


    void setNext(ListNode *next);

    void listPrint(type_t type);

    ListNode* searchListC(string str);

    ListNode* searchListD(string str);

    void insertList(Record* rec);

    ListNode(Record* rec);
    ~ListNode();
};


#endif //DISEASEMONITOR_LISTNODE_H
