#ifndef DISEASEMONITOR_RECORDIDMANAGEMENT_H
#define DISEASEMONITOR_RECORDIDMANAGEMENT_H

#include <iostream>
#include "Record.h"

using namespace std;

class ID_Node {
private:
    Record *record;
    ID_Node* next;

public:
    bool existsID(int i);

    Record *searchID(int id);

    void recordPatientExit(Date *exit);

    ID_Node(Record *record, ID_Node* next);

    ~ID_Node();
};

class ID_Bucket {
private:
    ID_Node* head;

public:
    void recordPatientExit(Date *exit);

    void insertID(Record *record);

    bool existsID(int i);

    Record *searchID(int id);

    ID_Bucket();

    ~ID_Bucket();
};

class ID_Hashtable {
private:
    int bucketsNum;
    ID_Bucket** table;
    int hash(int k) {
        return k % bucketsNum;
    }

public:
    void recordPatientExit(int id, Date *exit);

    bool existsID(int i);

    void insertID(Record *record);

    Record *searchID(int id);

    ID_Hashtable(int bucketsNum);

    ~ID_Hashtable();

};


#endif //DISEASEMONITOR_RECORDIDMANAGEMENT_H
