#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <dirent.h>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Tree.h"
#include "Bucket.h"
#include "Hashtable.h"
#include "Heap.h"
#include "RecordIDManagement.h"
#include "Functions.h"
#include "Record.h"
#include "Commands.h"
#include "SignalHandling.h"

#define SIZE 10
#define BUCKET_NUM 10
#define BUCKET_SIZE 256


int main(int argc, char* argv[]) {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = kill_child;
    act.sa_flags = SA_SIGINFO;
    signals = 0;
    int bufferSize = 512, fd , fd2, sent;
    string filePath, word, i, j, k;
    char filepath[20];
    string w, countryS;
    char *readbuf, *writebuf;
    char *myfifo, *auxfifo;

    ID_Hashtable *idHT = new ID_Hashtable(SIZE);
    Hashtable *diseaseHT = new Hashtable(BUCKET_NUM, BUCKET_SIZE, disease);
    Hashtable *countryHT = new Hashtable(BUCKET_NUM, BUCKET_SIZE, country);

    myfifo = create_fifo("myfifo", getpid());
    auxfifo = create_fifo("auxfifo", getpid());

    if (mkfifo(myfifo, 0666) == -1 && errno != EEXIST) {
        cout << "Error with main mkfifo: " << errno << endl;
        return errno;
    }
    fd = open(myfifo, O_RDONLY);

    if (mkfifo(auxfifo, 0666) == -1 && errno != EEXIST) {
        cout << "Error with main auxfifo: " << errno << endl;
    }
    fd2 = open(auxfifo, O_WRONLY);

    while (true) {
        if(read_line(fd, readbuf, bufferSize) != 0) {
            cout << "error in read" << endl;
            return errno;
        }
        if (strcmp(readbuf, "OK") == 0) break;
        else {
            sprintf(filepath, "%s", readbuf);
            char *c = strtok(readbuf, "/");
            c = strtok(NULL, "/");
//            string countryS(c);
            char *country=new char[strlen(c)+1];
            strcpy(country,c);
            initialize_record(filepath, country, diseaseHT, countryHT, idHT, fd2, bufferSize);
        }
    }
    write_line(fd2, writebuf, bufferSize, "OK");



    while(true) {
        if(signals == SIGUSR1) {
            cout << "Child process: " << getpid() << " killed by signal" << endl;
            break;
        }
        int size = 0;
        read(fd, &size, sizeof(int));
        if (read_line(fd, readbuf, size, bufferSize) != 0) {
            cout << "error in read" << endl;
            return errno;
        }
        string g(readbuf);
        select_command(diseaseHT, countryHT, idHT, filepath, g, fd2);
    }



    /* Closing and removing named pipes */
    if (close(fd) < 0) {
        cout << "Error on child process close(fd) with code: " << errno << endl;
    }
    if (close(fd2) < 0) {
        cout << "Error on child process close(fd2) with code: " << errno << endl;
    }
    unlink(myfifo);
    return 0;
}