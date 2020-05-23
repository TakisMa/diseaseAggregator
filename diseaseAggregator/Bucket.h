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

    int getFull() const;

    void setFull(int f) {
        Bucket::full = f;
    }

    Bucket *getNext() const {
        return next;
    }

    string getCountry() {
        if(head) return head->getCountry();
        else if(next) return next->getCountry();
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
