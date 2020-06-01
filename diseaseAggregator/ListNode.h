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
    int *ages;
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

    bool findNewFile(Date *filename, string country);

    string getCountry();

    Record *getRecord() const {
        return record;
    }

    void compareCountry(Record *rec) {
        if(record->getCountry().compare(rec->getCountry()) == 0);
    }

    void compareDisease(Record *rec) {
        if(record->getDiseaseId() == rec->getDiseaseId());
    }

    int *getAges(string virus, Date *date1, Date *date2, string country) {
        int total = 0;
        int *results = new int[4];
        for(int i = 0; i < 4; i ++) {
            total += ages[i];
            results[i] = 0;
        }
        cout << "total is: " << total << endl;
        if(record->getDiseaseId() == virus) {
            results[0] = tree->getAges(0, 20, date1, date2, country);
            cout << "result 0: " << results[0] << endl;
            results[1] = tree->getAges(21, 40, date1, date2, country);
            cout << "result 1: " << results[1] << endl;
            results[2] = tree->getAges(41, 60, date1, date2, country);
            cout << "result 2: " << results[2] << endl;
            results[3] = tree->getAges(61, 120, date1, date2, country);
            cout << "result 3: " << results[3] << endl;
            for(int i = 0; i < 4; i++) {
                results[i] = total / results[i];
                cout << "result " << i << " after division is: " << results[i] << endl;
            }
            return results;
        }
        else if(next) return next->getAges(virus, date1, date2, country);
        else {
            cout << "List will return null" << endl;
            return NULL;
        }

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
