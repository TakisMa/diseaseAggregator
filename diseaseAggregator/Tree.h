#ifndef DISEASEMONITOR_TREE_H
#define DISEASEMONITOR_TREE_H


#include <cstdlib>
#include "TreeNode.h"

class Tree {
private:
    TreeNode* root;
    int height;
    int count;

public:
    TreeNode* getRoot();
    int getHeight();

    void setRoot(TreeNode *root);
    void setHeight(int height);

    int getCount() const;

    void topk(string toCheck, Heap* heap, Date *entry, Date *exit, type_t type);

    void topk(string toCheck, Heap* heap, type_t type);

    void numCurrentPatients();

    void globalDiseaseStats(Date *entry, Date *exit);

    void globalDiseaseStats();

    int numPatientAdmissions(Date *date1, Date *date2, string countries);

    int numPatientDischarges(Date *date1, Date *date2, string countries);

    int countIncidents(Date *entry, Date *exit, string country, type_t type);

    int countIncidents(Date *entry, Date *exit, string country);

    int countIncidents(Date *entry, Date *exit);

    bool findNewFile(Date *filename) {
        if(root) return root->findNewFile(filename);
        else return false;
    }

    int getAges(int start, int end, Date *date1, Date *date2, string country) {
        if(root) return root->getAges(start, end, date1, date2, country);
    }

    void insertItem(Record *rec);

    TreeNode* searchItem(int item);
/*
    void inorderPrint();

    void preorderPrint();

    void postorderPrint();

    void testPrint();*/

    Tree();
    ~Tree();
};

#endif //DISEASEMONITOR_TREE_H

