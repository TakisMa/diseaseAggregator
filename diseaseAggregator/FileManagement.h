#ifndef DISEASEAGGREGATOR_FILEMANAGEMENT_H
#define DISEASEAGGREGATOR_FILEMANAGEMENT_H

#include <string>
#include <unistd.h>
#include <sys/types.h>
#define BUCKET_SIZE 256

using namespace std;

class FileList {
private:
    string country;
    pid_t pid;
    FileList *next;

public:
    void linsert(string item, pid_t p) {
        if(next) next->linsert(item, p);
        else next = new FileList(item, p);
    }

    pid_t lsearch(string item) {
        if(item == country) return pid;
        else if(next) return lsearch(item);
        else return -1;
    }

    FileList(string item, pid_t p) {
        country = item;
        pid = p;
        next = NULL;
    }
    ~FileList();

};

class FileHashtable {
private:
    FileList *bucket[BUCKET_SIZE];
    int hashS(string s) {
        int hash = 0;
        for(int i = 0; i < s.length(); i++)
            hash += s[i];
        return hash % BUCKET_SIZE;
    }

public:
    void hinsert(string item, pid_t p) {
        int h = hashS(item);
        if(bucket[h]) bucket[h]->linsert(item, p);
        else {
            bucket[h] = new FileList(item, p);
        }
    }

    pid_t hsearch(string item) {
        int h = hashS(item);
        if(bucket[h]) return bucket[h]->lsearch(item);
        else return -1;
    }

    FileHashtable() {
        for(int i = 0; i < BUCKET_SIZE; i++)
            bucket[i] = NULL;
    }
    ~FileHashtable();
};



#endif //DISEASEAGGREGATOR_FILEMANAGEMENT_H
