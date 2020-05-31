#include "ListNode.h"

void ListNode::topk(string country, Heap *heap, Date *entry, Date *exit, type_t type) {
    if(tree) tree->topk(country, heap, entry, exit, type);
    if(next) next->topk(country, heap, entry, exit, type);
}

void ListNode::topk(string country, Heap *heap, type_t type) {
    if(tree) tree->topk(country, heap, type);
    if(next) next->topk(country, heap, type);
}

void ListNode::numCurrentPatients(string disease) {
//    int sum = 0;
    if(record->getDiseaseId() == disease) {
        if(tree) tree->numCurrentPatients();
    }
//    else cout << disease << " 0" << endl;
    if(next) next->numCurrentPatients();
//    return sum;
}

void ListNode::numCurrentPatients() {
    if(tree) tree->numCurrentPatients();
    if(next) next->numCurrentPatients();
}

void ListNode::globalDiseaseStats(Date *entry, Date *exit) {
    if(tree) tree->globalDiseaseStats(entry, exit);
    if(next) next->globalDiseaseStats(entry, exit);
}

void ListNode::globalDiseaseStats() {
    if(tree) tree->globalDiseaseStats();
    if(next) next->globalDiseaseStats();
}

int ListNode::countIncidents(Date *entry, Date *exit, string country) {
    return tree->countIncidents(entry, exit, country);
}

int ListNode::countIncidents(Date *entry, Date *exit) {
    if(tree) return tree->countIncidents(entry, exit);
    else return 0;
}

void ListNode::setNext(ListNode *next) {
    this->next = next;
}

void ListNode::listPrint(type_t type) {
    if(type == disease) cout << record->getDiseaseId() << " -> ";
    if(type == country) cout << record->getCountry() << " -> ";
    if(next != NULL) next->listPrint(type);
}

ListNode *ListNode::searchListC(string str) {
    if(str == record->getCountry()) return this;
    else if(next) return next->searchListC(str);
    else return NULL;
}

ListNode *ListNode::searchListD(string str) {
    if(str == record->getDiseaseId()) return this;
    else if(next) return next->searchListD(str);
    else return NULL;
}

void ListNode::insertList(Record *rec) {
    if(tree) tree->insertItem(rec);
    else {
        tree = new Tree();
        tree->insertItem(rec);
    }
    if(rec->getAge() > 60) ages[3] ++;
    else if(rec->getAge() > 40) ages[2] ++;
    else if(rec->getAge() > 20) ages[1] ++;
    else if(rec->getAge() > 0) ages[0] ++;
    else cout << "Wrong age" << endl;
}



ListNode::ListNode(Record *rec) {
    next = NULL;
    tree = NULL;
    record = rec;
    ages = new int[4];
    for(int i = 0; i < 4; i++) {
        ages[i] = 0;
    }
}

ListNode::~ListNode() {
    delete next;
    delete tree;
    delete[] ages;
}

int ListNode::numPatientAdmissions(Date *date1, Date *date2, string countries) {
    return tree->numPatientAdmissions(date1, date2, countries);
}

int ListNode::numPatientDischarges(Date *date1, Date *date2, string countries) {
    return tree->numPatientDischarges(date1, date2, countries);
}
