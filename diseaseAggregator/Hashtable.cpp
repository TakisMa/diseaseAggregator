#include "Hashtable.h"

void Hashtable::topk(string country, Heap *heap, Date *entry, Date *exit) {
    for(int i = 0; i < bucketsNum; i++) {
        if(table[i])  table[i]->topk(country, heap, entry, exit, type);
    }
}

void Hashtable::topk(string country, Heap *heap) {
    for(int i = 0; i < bucketsNum; i++) {
        if(table[i])  table[i]->topk(country, heap, type);
    }
}

void Hashtable::numCurrentPatients(string disease) {
    if(table[hashS(disease)]) table[hashS(disease)]->numCurrentPatients(disease);
//    else cout << disease << " 0" << endl;
}

void Hashtable::numCurrentPatients() {
    for(int i = 0; i < bucketsNum; i++) {
        if(table[i]) table[i]->numCurrentPatients();
    }
}

void Hashtable::globalDiseaseStats() {
    for(int i = 0; i < bucketsNum; i++) {
        if(table[i]) table[i]->globalDiseaseStats();
    }
}

void Hashtable::globalDiseaseStats(Date *entry, Date *exit) {
    for(int i = 0; i < bucketsNum; i++) {
        if(table[i]) table[i]->globalDiseaseStats(entry, exit);
    }
}

int Hashtable::diseaseFrequency(string virus, Date *entry, Date *exit) {
    if(table[hashS(virus)]) return table[hashS(virus)]->countIncident(virus, entry, exit);
    else return 0;
}

int Hashtable::diseaseFrequencyC(string virus, Date *entry, Date *exit, string country) {
    if(table[hashS(virus)]) return table[hashS(virus)]->countIncident(virus, entry, exit, country);
    else return 0;
}

void Hashtable::insertHashTable(Record *rec) {
    int i = 0;
    if(type == disease) i = hashS(rec->getDiseaseId());
    else if(type == country) i = hashS(rec->getCountry());
    if(table[i] == NULL) {
        table[i] = new BucketList(bucketSize);
        table[i]->insertBucketList(rec, type);
    }
    else {
        table[i]->insertBucketList(rec, type);
    }
}

void Hashtable::hashPrint() {
    for(int i = 0; i < bucketsNum; i++) {
        cout << endl;
        cout << "-----------------------------------" << endl;
        cout << "Bucket " << i << ": " << endl;
        if(table[i] == NULL) {
            cout << "NULL" << endl;
            cout << "-----------------------------------" << endl;
            continue;
        }
        table[i]->PrintBucketList(type);
        cout << endl;
        cout << "-----------------------------------" << endl;

    }
}

void Hashtable::printTree() {
    for(int i = 0; i < bucketsNum; i++ ) {
        table[i]->PrintBucketList(type);
    }
}

Hashtable::Hashtable(int elements, int size, type_t t) {
    counter++;
    bucketsNum = elements;
    bucketSize = size;
    type = t;
    table = new BucketList*[bucketsNum];
    for(int i  = 0; i < bucketsNum; i++) {
        table[i] = NULL;
    }
}

Hashtable::~Hashtable() {
    counter--;
    for(int i = 0; i < bucketsNum; i++)
        delete table[i];
    delete[] table;
}
