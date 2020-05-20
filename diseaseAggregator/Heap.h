#ifndef DISEASEMONITOR_HEAP_H
#define DISEASEMONITOR_HEAP_H

#include <math.h>
#include "HeapNode.h"
enum type_t:bool{disease, country};

class Heap {
private:
    int height;
    int totalNodes;
    HeapNode* root;
    type_t type;

    int calcRowCur();

    bool isPow2(int x);

    int calcRowMax();

public:
    void topk(int k);

    void insertItem(string fill, int item);

    void printHeap(int k);

    Heap(type_t type);

    ~Heap();

};


#endif //DISEASEMONITOR_HEAP_H
