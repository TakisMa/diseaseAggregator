#include "BucketList.h"

void BucketList::topk(string country, Heap *heap, Date *entry, Date *exit, type_t type) {
    if(first) first->topk(country, heap, entry, exit, type);
}

void BucketList::topk(string country, Heap *heap, type_t type) {
    if(first) first->topk(country, heap, type);
}

void BucketList::numCurrentPatients(string disease) {
    if(first) first->numCurrentPatients(disease);
}

void BucketList::numCurrentPatients() {
    if(first) first->numCurrentPatients();
}

void BucketList::globalDiseaseStats() {
    if(first) first->globalDiseaseStats();
}

void BucketList::globalDiseaseStats(Date *entry, Date *exit) {
    if(first) first->globalDiseaseStats(entry, exit);
}

int BucketList::countIncident(string virus, Date *entry, Date *exit, string country) {
    if(first) return first->countIncident(virus, entry, exit, country);
    else return 0;
}

int BucketList::countIncident(string virus, Date *entry, Date *exit) {
    if(first) return first->countIncident(virus, entry, exit);
    else return 0;
}

void BucketList::insertBucketList(Record *rec, type_t t) {
    if(first == NULL){
        first = new Bucket(bucketSize);
        insertBucketList(rec, t);
    }
    else {
        Bucket* insertB = NULL;
        if(t == country) insertB = searchBucketC(rec->getCountry());
        else if(t == disease) insertB = searchBucketD(rec->getDiseaseId());
        if(insertB) insertB->insertBucket(rec, t);
        else if(!first->getFull()) first->insertBucket(rec, t);
        else {
            Bucket* tmp = first;
            first = new Bucket(bucketSize);
            first->setNext(tmp);
            insertBucketList(rec, t);
        }
    }
}

Bucket *BucketList::searchBucketC(string str) {
    if(first) return first->searchBucketC(str);
    else return NULL;
}

Bucket *BucketList::searchBucketD(string str) {
    if(first) return first->searchBucketD(str);
    else return NULL;
}

void BucketList::PrintBucketList(type_t type) {
    if(first) first->bucketPrint(type);
}

BucketList::BucketList(int size) {
    bucketSize = size;
    first = NULL;
}

BucketList::~BucketList() {
    delete first;
}

int BucketList::numPatientAdmissions(string virus, Date *date1, Date *date2, string countries) {
    if(first) return first->numPatientAdmissions(virus, date1, date2, countries);
    else return 0;
}

int BucketList::numPatientDischarges(string virus, Date *date1, Date *date2, string countries) {
    if(first) return first->numPatientDischarges(virus, date1, date2, countries);
    else return 0;
}
