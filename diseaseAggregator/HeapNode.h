#ifndef DISEASEMONITOR_HEAPNODE_H
#define DISEASEMONITOR_HEAPNODE_H

#include <iostream>
#include "Record.h"
using namespace std;

class HeapNode {
private:
//    Record* record;
    int total;
    HeapNode* left;
    HeapNode* right;
    string fill;

public:
    int getTotal() const;

    const string &getFill() const;

    void printHeap(int k);
    
    void swap(HeapNode *a, HeapNode *b);

    void heapify();

    bool extractMax(HeapNode *root);

    void insertHeapNode(string fill, int item, int row_max, int row_cur);

    /*HeapNode* insertHeapNode(Record *rec, int row_max, int row_cur) {
        if(row_cur <= row_max/2){
            if(!left)
                return left = new HeapNode(rec, item);
            else
                left->insertHeapNode(rec, item, row_max/2, row_cur);
        }
        else{
            if(!right)
                return right = new HeapNode(rec, item);
            else
                right->insertHeapNode(rec, item, row_max/2, row_cur - (row_max/2));
        }

    }*/

    HeapNode(string fill, int item);

    ~HeapNode();
};


#endif //DISEASEMONITOR_HEAPNODE_H
