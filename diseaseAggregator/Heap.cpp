#include "Heap.h"

void Heap::topk(int k) {
    for(int i = 0; i < k; i++) {
        if(root) {
            cout << root->getFill() << " " << root->getTotal() << endl;
            if (totalNodes > 1) {
                root->extractMax(root);
                totalNodes--;
            }
            else {
                delete root;
                root = NULL;
            }
        }
        else return;
    }
}

void Heap::insertItem(string fill, int item) {
    totalNodes++;
    if(root){
        int rowMax = calcRowMax();
        int rowCur = calcRowCur();
        root->insertHeapNode(fill, item, rowMax,rowCur);
        root->heapify();
    }
    else{
        root = new HeapNode(fill, item);
    }
}

/*void Heap::printHeap(int k) {
    if(type == disease)
        if(root) root->printHeap(k);
}*/

int Heap::calcRowCur() {
    int max = pow(2, height) - 1;
    return totalNodes - max;
}

bool Heap::isPow2(int x) {
    return x && !(x & (x - 1));
}

int Heap::calcRowMax() {
    if(isPow2(totalNodes)) height++;
    return 1 << height;
}


Heap::Heap(type_t type) {
    height = 0;
    totalNodes = 0;
    root = nullptr;
    this->type = type;
}

Heap::~Heap() {
    delete root;
}
