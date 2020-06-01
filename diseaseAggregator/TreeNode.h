#ifndef DISEASEMONITOR_TREENODE_H
#define DISEASEMONITOR_TREENODE_H

#include <iostream>
#include <cstdlib>
#include "Record.h"
#include "Heap.h"

#define COUNT 20
extern int counter;


using namespace std;

class TreeNode {
private:
    int data;
    Record *record;

    TreeNode* left_child;
    TreeNode* right_child;

public:
    int topk(string toCheck, type_t type);

    Record *getRecord() const;

    int numCurrentPatients();
    
    int numPatientAdmissions(Date *date1, Date *date2, string countries);

    int numPatientDischarges(Date *date1, Date *date2, string countries);

    int countIncidents(Date* entry, Date* exit, string toCheck);

    int countIncidents(Date* entry, Date* exit, string toCheck, type_t type);

    int countIncidents(Date* entry, Date* exit);

    bool findNewFile(Date *filename) {
        if((record->getEntryDate()->compare(filename) == 0) || record->getExitDate()->compare(filename) == 0) return true;
        if(left_child && left_child->findNewFile(filename)) return true;
        if(right_child && right_child->findNewFile(filename)) return true;
        return false;
    }

    int getAges(int startAge, int endAge, Date *date1, Date *date2, string country) {
        int r = 0, l = 0;
        if(record->getEntryDate()->compare(date1) == 1 && record->getExitDate()->compare(date2) == -1) {
            if(left_child) l = left_child->getAges(startAge, endAge, date1, date2, country);
            if(right_child) r = right_child->getAges(startAge, endAge, date1, date2, country);
            if(record->getCountry() == country) {
                if(record->getAge() >= startAge && record->getAge() <= endAge) return (r + l) + 1;
                else return r + l;
            }
            else return r + l;
        }
        else if(record->getEntryDate()->compare(date1) == -1) {
            if(right_child) r = right_child->getAges(startAge, endAge, date1, date2, country);
            return r;
        }
        else if(record->getEntryDate()->compare(date2) == 1) {
            if(left_child) l = left_child->getAges(startAge, endAge, date1, date2, country);
            return l;
        }
        else return 0;
    }

    TreeNode* rightRotation();

    TreeNode* leftRotation();

    int max(int a, int b);

    int isBalanced();

    int getHeight();

    TreeNode* insertTreeNode(Record *rec);

    TreeNode* searchTreeNode(int item);

    /*void InorderPrint();
    void PreorderPrint();
    void PostorderPrint();
    void testPrint();*/

    TreeNode* getMin();


    TreeNode(Record *record);
    ~TreeNode();


};


#endif //DISEASEMONITOR_TREENODE_H
