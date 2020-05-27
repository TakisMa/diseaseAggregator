#ifndef DISEASEAGGREGATOR_SUMMARYLIST_H
#define DISEASEAGGREGATOR_SUMMARYLIST_H

#include <string>
#include <iostream>
#include "Record.h"

using namespace std;

class Node {
private:
    string disease;
    int *ages;
    Node* next;
public:
    string getAges() {
        string summary;
        summary = summary + "?" +disease + "?\\";
        for(int i = 0; i < 4; i++) {
            if(i == 3) {
                summary = summary + to_string(ages[i]);
                continue;
            }
            summary = summary + to_string(ages[i]) + "?";
        }
        if(next) summary += next->getAges();
        else return summary;
    }

    Node *searchNode(string disease) {
        if(this->disease == disease) return this;
        else if(next) return next->searchNode(disease);
        else return NULL;
    }

    Node *insertNode(string disease, Record *record) {
        Node *pos = searchNode(record->getDiseaseId());
        if(!pos) pos = new Node(disease, this);
        if(record->getAge() > 60) pos->ages[3] ++;
        else if(record->getAge() > 40) pos->ages[2] ++;
        else if(record->getAge() > 20) pos->ages[1] ++;
        else if(record->getAge() > 0) pos->ages[0] ++;
        return pos;
    }

    Node(string disase, Node* next) {
        this->disease = disase;
        this->next = next;
        ages = new int[4];
        for(int i = 0; i < 4; i++) ages[i] = 0;
    }
    ~Node() {
        delete[] ages;
        delete next;
    }
};

class List {
private:
    string country;
    List* next;
    Node *head;
public:
    string getAges(string country) {
        List *pos = searchList(country);
        if(!pos) return NULL;
        else {
            return pos->head->getAges();
        }
    }

    List* searchList(string country) {
        if(this->country == country) return this;
        else if(next) return next->searchList(country);
        else return NULL;
    }
    List *insertList(string country, string disease, Record *record) {
        List *pos = searchList(country);
        if(!pos) pos = new List(this, disease, country);
        pos->head = pos->head->insertNode(disease, record);
        return pos;
    }

    List() {
        this->next = NULL;
        this->head = NULL;
    }

    List(List *next, string disease, string country) {
        this->country = country;
        this->next = next;
        this->head = new Node(disease, NULL);
    }
    ~List() {
        delete head;
        delete next;
    }
};

#endif //DISEASEAGGREGATOR_SUMMARYLIST_H
