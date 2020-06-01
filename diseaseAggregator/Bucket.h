#ifndef DISEASEMONITOR_BUCKET_H
#define DISEASEMONITOR_BUCKET_H


#include <cstdlib>
#include "Record.h"
#include "ListNode.h"


class Bucket {
private:
    int sum;
    int full;
    int bucketSize;
    ListNode* head;
    Bucket* next;

public:
    void topk(string country, Heap *heap, Date *entry, Date *exit, type_t type);

    void topk(string country, Heap *heap, type_t type);

    void numCurrentPatients(string disease);

    void numCurrentPatients();

    void globalDiseaseStats();

    void globalDiseaseStats(Date *entry, Date *exit);

    int numPatientAdmissions(string virus, Date *date1, Date *date2, string countries);

    int numPatientDischarges(string virus, Date *date1, Date *date2, string countries);

    int countIncident(string virus, Date *entry, Date *exit,string country);

    int countIncident(string virus, Date *entry, Date *exit);

    bool findNewFile(Date *filename, string country) {
        bool tmp = false;
        if(head) tmp = head->findNewFile(filename, country);
        if(!tmp && next) tmp = next->findNewFile(filename, country);
        return tmp;
    }

    int getFull() const;

    void setFull(int f) {
        Bucket::full = f;
    }

    Bucket *getNext() const {
        return next;
    }

    int *getAges(string virus, Date *date1, Date *date2, string country) {
        int *tmp = NULL;
        if(head) {
            cout << "head exists" << endl;
            tmp = head->getAges(virus, date1, date2, country);
        }
        if(tmp) {
            cout << "ready to return tmp" << endl;
            return tmp;
        }
        else if(next) {
            cout << "searching next Bucket" << endl;
            return next->getAges(virus, date1, date2, country);
        }
        else {
            cout << "returning NULL" << endl;
            return NULL;
        }
    }

    string getCountry() {
        string countries;
        if(head) {
            countries += head->getCountry();
        }
        if(next) countries += next->getCountry();
        return countries;
    }

    void setNext(Bucket *n);

    void insertBucket(Record *rec, type_t t);

    Bucket* searchBucketC(string str);

    Bucket* searchBucketD(string str);

    void bucketPrint(type_t type);

    Bucket(int size);

    ~Bucket();
};


#endif //DISEASEMONITOR_BUCKET_H
