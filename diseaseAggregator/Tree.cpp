#include "Tree.h"



int Tree::getHeight() {
    return height;
}

TreeNode *Tree::getRoot() {
    return root;
}

void Tree::setRoot(TreeNode *root) {
    Tree::root = root;
}

void Tree::setHeight(int height) {
    Tree::height = height;
}


Tree::~Tree() {
    delete root;
}

Tree::Tree() {
    count = 0;
    height = -1;
    root = NULL;
}

int Tree::getCount() const {
    return count;
}

void Tree::topk(string toCheck, Heap *heap, Date *entry, Date *exit, type_t type) {
    if(root) {
        int x = root->countIncidents(entry, exit, toCheck, type);
        if(type == disease) {
            if(x != 0) heap->insertItem(root->getRecord()->getDiseaseId(), x);
        }
        else if(type == country) {
            if(x != 0) heap->insertItem(root->getRecord()->getCountry(), x);
        }
    }
}

void Tree::topk(string toCheck, Heap *heap, type_t type) {
    if(root) {
        int x = root->topk(toCheck, type);
        if(type == disease) {
            if(x != 0) heap->insertItem(root->getRecord()->getDiseaseId(), x);
        }
        else if(type == country) {
            if(x != 0) heap->insertItem(root->getRecord()->getCountry(), x);
        }
    }
}

void Tree::numCurrentPatients() {
    if(root) {
        cout << root->getRecord()->getDiseaseId() << " " << root->numCurrentPatients() << endl;
    }
}

void Tree::globalDiseaseStats(Date *entry, Date *exit) {
    if(root) {
        cout << root->getRecord()->getDiseaseId() << " " << root->countIncidents(entry, exit)  <<  endl;
    }
}

void Tree::globalDiseaseStats() {
    if(root) cout << root->getRecord()->getDiseaseId() << " " << count << endl;
}

void Tree::insertItem(Record *rec) {
    if(root != NULL) {
        root = root->insertTreeNode(rec);
    }
    else{
        root = new TreeNode(rec);
    }
    count ++;
}

int Tree::countIncidents(Date *entry, Date *exit, string country, type_t type) {
    return root->countIncidents(entry, exit, country, type);
}

int Tree::countIncidents(Date *entry, Date *exit, string country) {
    return root->countIncidents(entry, exit, country);
}

int Tree::countIncidents(Date *entry, Date *exit) {
    if(root) return root->countIncidents(entry, exit);
    else return 0;
}

TreeNode *Tree::searchItem(int item) {
    return root->searchTreeNode(item);
}

int Tree::numPatientAdmissions(Date *date1, Date *date2, string countries) {
    return root->numPatientAdmissions(date1, date2, countries);
}

int Tree::numPatientDischarges(Date *date1, Date *date2, string countries) {
    return root->numPatientDischarges(date1, date2, countries);
}

/*void Tree::inorderPrint() {
    root->InorderPrint();
}

void Tree::preorderPrint() {
    root->PreorderPrint();
}

void Tree::postorderPrint() {
    root->InorderPrint();
}

void Tree::testPrint() {
    root->testPrint();
    cout << "Count = " << count << endl;
}*/


