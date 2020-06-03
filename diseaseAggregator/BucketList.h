#ifndef DISEASEMONITOR_BUCKETLIST_H
#define DISEASEMONITOR_BUCKETLIST_H


#include "Bucket.h"
#include "Record.h"

class BucketList {
private:
    Bucket* first;
    int bucketSize;
    int sum;
public:
    void topk(string country, Heap* heap, Date *entry, Date *exit, type_t type);

    void topk(string country, Heap* heap, type_t type);

    void numCurrentPatients(string disease);

    void numCurrentPatients();

    void globalDiseaseStats();

    void globalDiseaseStats(Date *entry, Date *exit);

    int numPatientAdmissions(string virus, Date *date1, Date *date2, string countries);

    int numPatientDischarges(string virus, Date *date1, Date *date2, string countries);

    string getCountry() {
        if(first) return first->getCountry();
        else return "";
    }


    int countIncident(string virus, Date *entry, Date *exit, string country);

    int countIncident(string virus, Date *entry, Date *exit);

    bool findNewFile(Date *filename, string country) {
        if(first) return first->findNewFile(filename, country);
        else return false;
    }

    int *getAges(string virus, Date *date1, Date *date2, string country) {
        if(first) return first->getAges(virus, date1, date2, country);
        else return NULL;
    }

    void insertBucketList(Record *rec,type_t t);

    Bucket* searchBucketC(string str);

    Bucket* searchBucketD(string str);

    void PrintBucketList(type_t type);

    BucketList(int size);

    ~BucketList();
};



#endif //DISEASEMONITOR_BUCKETLIST_H
