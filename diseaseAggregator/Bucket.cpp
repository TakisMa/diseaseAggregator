#include "Bucket.h"

void Bucket::topk(string country, Heap *heap, Date *entry, Date *exit, type_t type) {
    if(head) head->topk(country, heap, entry, exit, type);
    if(next) next->topk(country, heap, entry, exit, type);
}

void Bucket::topk(string country, Heap *heap, type_t type) {
    if(head) head->topk(country, heap, type);
    if(next) next->topk(country, heap, type);
}

void Bucket::numCurrentPatients(string disease) {
//    int sum = 0;
    if(head) head->numCurrentPatients(disease);
    if(next) next->numCurrentPatients(disease);
//    return sum;
}

void Bucket::numCurrentPatients() {
    if(head) head->numCurrentPatients();
    if(next) next->numCurrentPatients();
}

void Bucket::globalDiseaseStats() {
    if(head) head->globalDiseaseStats();
    if(next) next->globalDiseaseStats();
}

void Bucket::globalDiseaseStats(Date *entry, Date *exit) {
    if(head) head->globalDiseaseStats(entry, exit);
    if(next) next->globalDiseaseStats(entry, exit);
}

int Bucket::countIncident(string virus, Date *entry, Date *exit, string country) {
    ListNode *target = NULL;
    int sum = 0;
    if(head) target = head->searchListD(virus);
    if(target) sum = target->countIncidents(entry, exit, country);
    return sum;
}

int Bucket::countIncident(string virus, Date *entry, Date *exit) {
    ListNode *target = NULL;
    int sum = 0;
    if(head) target = head->searchListD(virus);
    if(target) sum = target->countIncidents(entry, exit);
    return sum;
}

int Bucket::getFull() const {
    return full;
}

void Bucket::setNext(Bucket *n) {
    Bucket::next = n;
}

void Bucket::insertBucket(Record *rec, type_t t) {
    if(head != NULL){
        ListNode* insertPos = NULL;
        if(t == country) insertPos = head->searchListC(rec->getCountry());
        else if(t == disease) insertPos = head->searchListD(rec->getDiseaseId());
        if(insertPos) insertPos->insertList(rec);
        else{
            ListNode* newNode = new ListNode(rec);
            newNode->setNext(head);
            head = newNode;
            head->insertList(rec);
        }
    }
    else{
        head = new ListNode(rec);
        head->insertList(rec);
    }
    sum += sizeof(ListNode);
    if(sum == bucketSize) full = 1;
}

Bucket *Bucket::searchBucketC(string str) {
    if(head){
        head->searchListC(str);
        return this;
    }
    else if(next) return next->searchBucketC(str);
    else return NULL;
}

Bucket *Bucket::searchBucketD(string str) {
    if(head){
        head->searchListD(str);
        return this;
    }
    else if(next) return next->searchBucketD(str);
    else return NULL;
}

void Bucket::bucketPrint(type_t type) {
    if(head != NULL) head->listPrint(type);
    if(next) next->bucketPrint(type);
}

Bucket::Bucket(int size) {
    sum = 0;
    full = 0;
    bucketSize = size;
    head = NULL;
    next = NULL;

}

Bucket::~Bucket() {
    delete head;
    delete next;
}

int Bucket::numPatientAdmissions(string virus, Date *date1, Date *date2, string countries) {
    ListNode *target = NULL;
    if(head) target = head->searchListD(virus);
    if(target) return target->numPatientAdmissions(date1, date2, countries);
    else return 0;
}

int Bucket::numPatientDischarges(string virus, Date *date1, Date *date2, string countries) {
    ListNode *target = NULL;
    if(head) target = head->searchListD(virus);
    if(target) return target->numPatientDischarges(date1, date2, countries);
    else return 0;
}
