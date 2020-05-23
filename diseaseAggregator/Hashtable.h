#ifndef DISEASEMONITOR_HASHTABLE_H
#define DISEASEMONITOR_HASHTABLE_H



#include "Bucket.h"
#include "BucketList.h"
#include "Record.h"
#include "Tree.h"
//#include "Heap.h"

class Hashtable {
private:
//    static int counter;
    type_t type;
    int bucketsNum;
    int bucketSize;
    BucketList** table;
    int hashS(string s) {
        int hash = 0;
        for(size_t i = 0; i < s.length(); i++)
            hash += s[i];
        return hash % bucketsNum;
    }

public:
    void topk(string country, Heap *heap, Date *entry, Date *exit);

    void topk(string country, Heap *heap);

    void numCurrentPatients(string disease);

    void numCurrentPatients();

    void globalDiseaseStats();

    void globalDiseaseStats(Date *entry, Date *exit);

    int numPatientAdmissions(string virus, Date *date1, Date *date2, string countries) {
        return table[hashS(virus)]->numPatientAdmissions(virus, date1, date2, countries);
    }

    int numPatientDischarges(string virus, Date *date1, Date *date2, string countries) {
        return table[hashS(virus)]->numPatientDischarges(virus, date1, date2, countries);
    }

    int diseaseFrequency(string virus, Date *entry, Date *exit);

    int diseaseFrequencyC(string virus, Date *entry, Date *exit, string country);

    string getCountry() {
        char *countries;
        for(int i = 0; i < bucketsNum; i++) {
            if(table[i]) table[i]->getCountry();
        }
    }

    void insertHashTable(Record *rec);

    void hashPrint();

    void printTree();

    Hashtable(int elements, int size, type_t t);

    ~Hashtable();
};


#endif //DISEASEMONITOR_HASHTABLE_H
