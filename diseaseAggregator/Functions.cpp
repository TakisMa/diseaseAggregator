#include <string>
#include "Functions.h"
#include "Record.h"
#include <iostream>
#include <cstring>

using namespace std;

int checkArguments(int argc, char **argv, int &workerNum, int &bufferSize, string &path) {
    bool arP = false;
    bool arW = false;
    bool arB = false;

    if(argc != 7) {
        cout << "Wrong arguments!" << endl;
        return -1;
    }
    else {
        for(int i = 1; i < argc; i += 2 ) {
            if((strcmp("-i", argv[i]) == 0) && !arP){
                path = string(argv[i+1]);
                arP = true;
            }
            else if((strcmp("-w", argv[i]) == 0) && !arW){
                if(i + 1 < 9){
                    if(!check_int(argv[i+1])) {
                        cout << "Argument after -h1 is not an INT!" << endl;
                        return -3;
                    }
                    else {
                        workerNum = atoi(argv[i+1]);
                        arW = true;
                    }
                }
                else {
                    cout << "Wrong arguments!" << endl;
                    return -1;
                }
            }
            else if((strcmp("-b", argv[i]) == 0) && !arB) {
                if(i + 1 < 9){
                    if(!check_int(argv[i+1])) {
                        cout << "Argument after -b is not an INT!" << endl;
                        return -3;
                    }
                    else {
                        bufferSize = atoi(argv[i+1]);
                        arB = true;
                    }
                }
                else {
                    cout << "Wrong arguments!" << endl;
                    return -1;
                }
            }
        }
        if(!arB || !arW || !arB) {
            cout << "Wrong arguments!" << endl;
            return -1;
        }
        else if(bufferSize == 0 || workerNum == 0){
            cout << "Wrong Hashtable parameters " << endl;
            return -2;
        }
    }
    return 0;
}

bool check_int(string num) { //checks if num is a number
    int i=0;
    do{
        if(num[i]<'0' || num[i]>'9') return false;
        i++;
    }while(num[i]!='\0');
    return true;
}

void swapD(struct Date *a, struct Date *b) {
    struct Date *tmp;
    tmp->year = a->year;
    tmp->month = a->month;
    tmp->day = a->day;
    a->year = b->year;
    a->month = b->month;
    a->day = b->day;
    b->year = tmp->year;
    b->month = tmp->month;
    b->day = tmp->day;

}

int partition(struct Date **array, int low, int high) {
    struct Date *pivot = array[high];
    int i = low - 1;
    for(int j = low; j <= high - 1; j++) {
        if(array[j]->compare(pivot) == -1) {
            i++;
            swapD(array[i], array[j]);
        }
    }
    swap(array[i + 1], array[high]);
    return (i+1);
}

void quickS(struct Date **array, int low, int high) {
    int pi;
    if(low < high) {
        pi = partition(array, low, high);
        quickS(array, low, pi - 1);
        quickS(array, pi+1, high);
    }
}
