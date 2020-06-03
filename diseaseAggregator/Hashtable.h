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

    bool findNewFile(Date *filename, char *cur_country) {
        bool tmp = false;
        string cc(cur_country);
        int pos = hashS(cc);
        if(table[pos]) tmp = table[pos]->findNewFile(filename, cc);
        return tmp;
    }

    int numPatientAdmissions(string virus, Date *date1, Date *date2, string countries) {
        int pos = hashS(virus);
        if(table[pos]) return table[pos]->numPatientAdmissions(virus, date1, date2, countries);
        return 0;
    }

    int numPatientDischarges(string virus, Date *date1, Date *date2, string countries) {
        int pos = hashS(virus);
        if(table[pos]) return table[hashS(virus)]->numPatientDischarges(virus, date1, date2, countries);
        return 0;
    }

    int diseaseFrequency(string virus, Date *entry, Date *exit);

    int diseaseFrequencyC(string virus, Date *entry, Date *exit, string country);

    string getCountry() {
        string countries;
        for(int i = 0; i < bucketsNum; i++) {
            if(table[i]) countries += table[i]->getCountry();
        }
        return countries;
    }

    int *getAges(string virus, Date *date1, Date *date2, string country) {
        int pos = hashS(virus);
        if(table[pos]) return table[pos]->getAges(virus, date1, date2, country);
        else return NULL;
    }

    void insertHashTable(Record *rec);

    void hashPrint();

    void printTree();

    Hashtable(int elements, int size, type_t t);

    ~Hashtable();
};


#endif //DISEASEMONITOR_HASHTABLE_H
