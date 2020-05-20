#include "RecordIDManagement.h"

bool ID_Node::existsID(int i) {
    if(i == record->getRecordId()) return true;
    else if(next) return next->existsID(i);
    else return false;
}

ID_Node::ID_Node(Record *rec, ID_Node *next) {
    this->record = rec;
    this->next = next;
}

ID_Node::~ID_Node() {
    delete next;
}

void ID_Node::recordPatientExit(Date *exit) {
    if(record->getEntryDate()->compare(exit) <= 0) {
        record->setExitDate(exit);
        cout << "Record updated" << endl;
    }
    else cout << "Please insert valid Date " << endl;
}

Record *ID_Node::searchID(int id) {
    if(id == record->getRecordId()) return record;
    else if(next) return next->searchID(id);
    else return NULL;
}

void ID_Bucket::insertID(Record *rec) {
    head = new ID_Node(rec , head);
}

bool ID_Bucket::existsID(int i) {
    if(head) return head->existsID(i);
    else return false;
}

ID_Bucket::ID_Bucket() {
    head = nullptr;
}

ID_Bucket::~ID_Bucket() {
    delete head;
}

void ID_Bucket::recordPatientExit(Date *exit) {
    if(head) head->recordPatientExit(exit);
}

Record *ID_Bucket::searchID(int id) {
    if(head) return head->searchID(id);
}

bool ID_Hashtable::existsID(int i) {
    if(table[hash(i)]) return table[hash(i)]->existsID(i);
    else return false;
}

ID_Hashtable::ID_Hashtable(int bucketsNum) {
    this->bucketsNum = bucketsNum;
    table = new ID_Bucket*[bucketsNum];
    for(int i = 0; i < bucketsNum; i++) {
        table[i] = nullptr;
    }
}

ID_Hashtable::~ID_Hashtable() {
    for(int i = 0; i < bucketsNum; i++) {
        delete table[i];
    }
    delete []table;
}

void ID_Hashtable::insertID(Record *record) {
    if(!table[hash(record->getRecordId())]) table[hash(record->getRecordId())] = new ID_Bucket;
    table[hash(record->getRecordId())]->insertID(record);
}

void ID_Hashtable::recordPatientExit(int id, Date *exit) {
    if(table[hash(id)]) table[hash(id)]->recordPatientExit(exit);
}

Record *ID_Hashtable::searchID(int id) {
    int h = hash(id);
    if(!table[h]) return NULL;
    else {
        return table[h]->searchID(id);
    }
}
