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
#define DISEASE_NUM 10
#define COUNTRY_NUM 10
#define BUCKET_SIZE 256


int main(int argc, char* argv[]) {
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = kill_child;
    act.sa_flags = SA_SIGINFO;
    if(sigaction(SIGUSR1, &act, NULL) == - 1) cout << "Error with sigaction: " << errno << endl;
    signals = 0;
    int bufferSize = 20, fd , fd2, sent;
    string filePath, word, i, j, k;
    char filepath[20];
    char command[30];
    string w, countryS;
    char *readbuf, *writebuf;
    char *myfifo, *auxfifo;

    ID_Hashtable *idHT = new ID_Hashtable(SIZE);
    Hashtable *diseaseHT = new Hashtable(DISEASE_NUM, BUCKET_SIZE, disease);
    Hashtable *countryHT = new Hashtable(COUNTRY_NUM, BUCKET_SIZE, country);


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
//    fd2 = open(auxfifo, O_WRONLY);


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
            string countryS(c);
            char *country=new char[strlen(c)+1];
            strcpy(country,c);
            initialize_record(filepath, country, diseaseHT, countryHT, idHT);
        }
    }
    select_command(diseaseHT, countryHT, idHT, filepath, "/numPatientDischarges COVID-2019 12-12-1990 12-12-2020 Italy", fd2);
    return 0;

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
        cout << "read size child process: " << size << endl;
        string g(readbuf);
        select_command(diseaseHT, countryHT, idHT, filepath, g, fd2);
        int k = write(fd2, &sent, sizeof(int));
        cout << "child process write(fd2) k = " << k << " & sent = " << sent << endl;
        while (sent != 0) {
            sent -= write(fd2, "OK", sent);
        }
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