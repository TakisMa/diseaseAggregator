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
