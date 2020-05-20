#include "HeapNode.h"

int HeapNode::getTotal() const {
    return total;
}

const string &HeapNode::getFill() const {
    return fill;
}

void HeapNode::printHeap(int k) {
    cout << "ParentNode: " << total << " " << fill << endl;
    if(left != NULL) cout << " LeftChildNode: " << left->total << " " << left->fill <<endl;
    if(right != NULL) cout << " RightChildNode: " << right->total << " " << right->fill << endl;
    else cout << endl;
    if(left != NULL) left->printHeap(k);
    if(right != NULL) right->printHeap(k);
}

void HeapNode::swap(HeapNode *a, HeapNode *b) {
    string cs = a->fill;
    int ci = a->total;
    a->fill = b->fill;
    a->total = b->total;
    b->fill = cs;
    b->total = ci;
}

void HeapNode::heapify() {
    if(left)
        if(total <= left->total) swap(this, left);
    if(right)
        if(total <= right->total) swap(this, right);
}

bool HeapNode::extractMax(HeapNode *root) {
    if(right) {
        if(right->extractMax(root)) {
            delete right;
            right = NULL;
        }
    }
    else if(left) {
        if(left->extractMax(root)) {
            delete left;
            left = NULL;
        }
    }
    else if(!right && !left) {
        swap(this, root);
        return true;
    }
    heapify();
    return false;
}

void HeapNode::insertHeapNode(string fill, int item, int row_max, int row_cur) {
    if(row_cur <= row_max/2){
        if(!left){
            left = new HeapNode(fill, item);
            heapify();
        }
        else{
            left->insertHeapNode(fill, item, row_max/2, row_cur);
            heapify();
        }
    }
    else{
        if(!right) {
            right = new HeapNode(fill, item);
            heapify();
        }
        else{
            right->insertHeapNode(fill, item, row_max/2, row_cur - (row_max/2));
            heapify();
        }
    }
}

HeapNode::HeapNode(string fill, int item) {
    right = nullptr;
    left = nullptr;
    total = item;
    this->fill = fill;
}

HeapNode::~HeapNode() {
    delete left;
    delete right;
}
