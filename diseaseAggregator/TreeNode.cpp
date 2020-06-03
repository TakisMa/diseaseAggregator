#include "TreeNode.h"

int counter;

TreeNode::TreeNode(Record *r) {
    record = r;
    left_child = NULL;
    right_child = NULL;
}

TreeNode::~TreeNode() {
    delete left_child;
    delete right_child;
    if(counter == 0) delete record;
}

Record *TreeNode::getRecord() const {
    return record;
}

int TreeNode::topk(string toCheck, type_t type) {
    int r = 0, l = 0;
    if(left_child) l = left_child->topk(toCheck, type);
    if(right_child) r = right_child->topk(toCheck, type);
    if(type == disease) {
        if(record->getCountry() == toCheck) return r + l + 1;
    }
    else if(type == country) {
        if(record->getDiseaseId() == toCheck) return r + l + 1;
    }
    return r + l;
}

int TreeNode::numCurrentPatients() {
    int l = 0, r = 0;
    if(left_child) l = left_child->numCurrentPatients();
    if(right_child) r = right_child->numCurrentPatients();
    if(!record->isDischarged()) return (r + l) + 1;
    else return r + l;
}

int TreeNode::countIncidents(Date *entry, Date *exit, string toCheck) {
    int l = 0, r = 0;
    if(record->getEntryDate()->compare(entry) == 1 && (record->getEntryDate()->compare(exit)) == -1) {
        if(left_child) l = left_child->countIncidents(entry, exit, toCheck);
        if(right_child) r = right_child->countIncidents(entry, exit, toCheck);
        if(toCheck == record->getCountry()) return (r + l) + 1;
        else return r + l;
    }
    else if(record->getEntryDate()->compare(entry) == -1) {
        if(right_child) r = right_child->countIncidents(entry, exit, toCheck);
        return r;
    }
    else if(record->getEntryDate()->compare(exit) == 1) {
        if(left_child) l = left_child->countIncidents(entry, exit, toCheck);
        return l;
    }
    else return 0;
}

int TreeNode::countIncidents(Date *entry, Date *exit, string toCheck, type_t type) {
    int l = 0, r = 0;
    if(record->getEntryDate()->compare(entry) == 1 && (record->getEntryDate()->compare(exit)) == -1) {
        if(left_child) l = left_child->countIncidents(entry, exit, toCheck, type);
        if(right_child) r = right_child->countIncidents(entry, exit, toCheck, type);
        if(type == disease) {
            if(toCheck == record->getCountry()) return (r + l) + 1;
        }
        else if(type == country) {
            if(toCheck == record->getDiseaseId()) return (r + l) + 1;
        }
        return r + l;
    }
    else if(record->getEntryDate()->compare(entry) == -1) {
        if(right_child) r = right_child->countIncidents(entry, exit, toCheck, type);
        return r;
    }
    else if(record->getEntryDate()->compare(exit) == 1) {
        if(left_child) l = left_child->countIncidents(entry, exit, toCheck, type);
        return l;
    }
    return 0;
}

int TreeNode::countIncidents(Date *entry, Date *exit) {
    int l = 0, r = 0;
    if(record->getEntryDate()->compare(entry) >= 0 && (record->getExitDate()->compare(exit)) <= 0) {
        if(left_child) l += left_child->countIncidents(entry, exit);
        if(right_child) r += right_child->countIncidents(entry, exit);
        return (r + l) + 1;
    }
    else if(record->getEntryDate()->compare(entry) == -1) {
        if(right_child) r = right_child->countIncidents(entry, exit);
        return r;
    }
    else if(record->getEntryDate()->compare(exit) == 1) {
        if(left_child) l = left_child->countIncidents(entry, exit);
        return l;
    }
    return 0;
}

TreeNode *TreeNode::rightRotation() {
    TreeNode* tmp = left_child;
    left_child = left_child->right_child;
    tmp->right_child = this;
    return tmp;
}

TreeNode *TreeNode::leftRotation() {
    TreeNode* tmp = right_child;
    right_child = right_child->left_child;
    tmp->left_child = this;
    return tmp;
}

int TreeNode::max(int a, int b) {
    return (a > b)? a : b;
}

int TreeNode::isBalanced() {
    if(left_child != NULL && right_child != NULL)
        return left_child->getHeight() - abs(right_child->getHeight());
    else if(left_child != NULL)
        return left_child->getHeight();
    else if(right_child != NULL)
        return -abs(right_child->getHeight());
    else return 0;
}

int TreeNode::getHeight() {
    int l = 0 , r = 0;
    if(left_child != NULL){
        l = left_child->getHeight();
    }
    if(right_child != NULL){
        r = right_child->getHeight();
    }
    return max(r, l) + 1;
}

TreeNode *TreeNode::insertTreeNode(Record *rec) {
    if(record->getEntryDate()->compare(rec->getEntryDate()) < 0) {
        if(right_child != NULL) {
            right_child = right_child->insertTreeNode(rec);
        }
        else {
            right_child = new TreeNode(rec);
        }
    }
    else if(record->getEntryDate()->compare(rec->getEntryDate()) >= 0) {
        if(left_child != NULL) {
            left_child = left_child->insertTreeNode(rec);
        }
        else {
            left_child = new TreeNode(rec);
        }
    }
    return this;
}

TreeNode *TreeNode::searchTreeNode(int item) {
    if (data == item) {
        return this;
    }
    else if (data > item) {
        if (left_child != NULL) {
            return left_child->searchTreeNode(item);
        }
        /*else {
            cout << "Item " << item << " not found! " << endl;
            return NULL;
        }*/
    }
    else if (data < item) {
        if (right_child != NULL) {
            return right_child->searchTreeNode(item);
        }
        /*else {
            cout << "Item " << item << " not found!" << endl;
            return NULL;
        }*/
    }
    return NULL;
}

/*void TreeNode::InorderPrint() {
    if(left_child != NULL) left_child->InorderPrint();
    cout << data << endl;
    if(right_child != NULL) right_child->InorderPrint();
}

void TreeNode::PreorderPrint() {
    cout << data << endl;
    if(left_child != NULL) left_child->PreorderPrint();
    if(right_child != NULL) right_child->PreorderPrint();

}

void TreeNode::PostorderPrint() {
    if(left_child != NULL) left_child->PostorderPrint();
    if(right_child != NULL) right_child->PostorderPrint();
    cout << data << endl;
}

void TreeNode::testPrint() {
    cout << "ParentNode: " << record->getEntryDate()->day << "-" << record->getEntryDate()->month << "-" << record->getEntryDate()->year << endl;
    if(right_child != NULL) cout << " RightChildNode: " << right_child->record->getEntryDate()->day << "-" << right_child->record->getEntryDate()->month << "-" << right_child->record->getEntryDate()->year << endl;
    if(left_child != NULL) cout << " LeftChildNode: " << left_child->record->getEntryDate()->day << "-" << left_child->record->getEntryDate()->month << "-" << left_child->record->getEntryDate()->year << endl;
    else cout << endl;
    if(left_child != NULL) left_child->testPrint();
    if(right_child != NULL) right_child->testPrint();
}*/

TreeNode *TreeNode::getMin() {
    if(left_child != NULL)
        left_child->getMin();
    else
        return this;
    return NULL;
}

int TreeNode::numPatientDischarges(Date *date1, Date *date2, string countries) {
    int c=0;
    if(getRecord()->getExitDate()->compare(date2) == -1 && getRecord()->getExitDate()->compare(date1) == 1 && getRecord()->getCountry() == countries) c++;
    if(right_child) c += right_child->numPatientDischarges(date1, date2, countries);
    if(left_child) c += left_child->numPatientDischarges(date1, date2, countries);
    return c;
}

int TreeNode::numPatientAdmissions(Date *date1, Date *date2, string countries) {
    int l = 0, r = 0;
    if(record->getEntryDate()->compare(date1) == 1 && (record->getEntryDate()->compare(date2)) == -1) {
        if (left_child) l = left_child->numPatientAdmissions(date1, date2, countries);
        if (right_child) r = right_child->numPatientAdmissions(date1, date2, countries);
        if(record->getCountry() == countries && (record->getState() == "ENTER") ) return (r + l) + 1;
        else return r + l;
    }
    else if(record->getEntryDate()->compare(date1) == -1) {
        if(right_child) r = right_child->numPatientAdmissions(date1, date2, countries);
        return r;
    }
    else if(record->getEntryDate()->compare(date2) == 1) {
        if(left_child) l = left_child->numPatientAdmissions(date1, date2, countries);
        return l;
    }
    else return 0;
}
