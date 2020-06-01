#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <string>
#include <sstream>
#include "Functions.h"
#include "SummaryManagement.h"
#include "SignalHandling.h"
#include "FileManagement.h"

#define BUCKETS_NUM 10
using namespace std;

int main(int argc, char *argv[]) {
    /* Check arguments */
    int numWorkers, bufferSize;
    string filePath;
    if(checkArguments(argc, argv, numWorkers, bufferSize, filePath ) != 0) {
        return -10;
    }

    struct sigaction child_act;
    sigemptyset(&child_act.sa_mask);
    child_act.sa_flags = SA_SIGINFO | SA_RESTART;
    child_act.sa_sigaction = child_int;
    if(sigaction(SIGCHLD, &child_act, NULL) == - 1) cout << "Error with sigaction: " << errno << endl;

    signals = -1;

    int wstatus;
    string w;
    pid_t *childpid = new pid_t[numWorkers];
    pid_t parentpid = getpid();
    int dir_count = 0;
    DIR * dirp;
    struct dirent * entry;
    int sent, size;
    int lastElement;
    char * tmp;
    char *readbuf;
    char *writebuf;
    char **myfifo = new char*[numWorkers];
    char **auxfifo = new char*[numWorkers];
    int *fd = new int[numWorkers];
    int *fd2 = new int[numWorkers];

    WHashtable *workerM = new WHashtable(BUCKETS_NUM);

    /* Determine number of directories */
    const char *cfilepath = filePath.c_str();
    dirp = opendir(cfilepath);
    if(!dirp) {
        cout << "Failed to open directory" << endl;
        return errno;
    }
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
            dir_count++;
        }
    }
    closedir(dirp);


    /* Create/execute children process equal to numWorkers and store pids in childpid array */
    char program[]="./worker";
    char *argumentsv[] = {program, NULL};
    for(int i = 0; i < numWorkers; i++) {
        childpid[i] = fork();
        if(childpid[i] < 0) {
            cout << "Fork Failed" << endl;
            return errno;
        }
        else if(childpid[i] > 0) {
            myfifo[i] = create_fifo("myfifo", childpid[i]);
            if(mkfifo(myfifo[i], 0666) == -1 && errno != EEXIST) {
                cout << "Error with main myfifo: " << errno << endl;
            }
            fd[i] = open(myfifo[i], O_WRONLY);

            auxfifo[i] = create_fifo("auxfifo", childpid[i]);
            if(mkfifo(auxfifo[i], 0666) == -1 && errno != EEXIST) {
                cout << "Error with main auxfifo: " << errno << endl;
            }
            fd2[i] = open(auxfifo[i], O_RDONLY);

            cout << childpid[i] << " fd = " << fd[i] << " fd2 = " << fd2[i] << endl;
        }
        if(childpid[i] == 0) {
            if(execvp("./cmake-build-debug/worker", argumentsv) == -1) {
                cout << "Exec failed " << endl;
                return errno;
            }
        }
    }
    if((dirp = opendir(cfilepath)) != NULL) {
        int pos = 0;
        while ((entry = readdir(dirp)) != NULL) {
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0) {
                    workerM->insertSummary(entry->d_name, fd[pos], fd2[pos], childpid[pos]);

                    /* send path to child process */
                    char *tosend = new char[strlen(cfilepath) + strlen(entry->d_name) + 2];
                    sprintf(tosend, "%s/%s", cfilepath, entry->d_name);
                    write_line(fd[pos], writebuf, bufferSize, tosend);
                    delete[] tosend;

                    /* read & print summary statics */
                    cout << "fd/fd2/pos: " << fd[pos] << " " << fd2[pos] << " " << pos << endl;
                    read_line(fd2[pos], readbuf, bufferSize);
                    char *c = new char[strlen(readbuf) + 1];
                    strcpy(c, readbuf);
                    c[strlen(readbuf)] = '\0';
                    print_summary(c);

                    delete[] readbuf;
                    if(pos == numWorkers - 1) pos = 0;
                    else pos++;
            }
        }
        cout << endl;
        lastElement = pos;
        closedir(dirp);
    }
    for(int i = 0; i < numWorkers; i++) write_line(fd[i], writebuf, bufferSize, "OK");
    int pos = 0;
    while(true) {
        read_line(fd2[pos], readbuf, bufferSize);
        if ((strcmp(readbuf, "OK") == 0) && pos == numWorkers-1) break;
        else if ((strcmp(readbuf, "OK") == 0)) pos++;
    }

    while(true) {
        if(signals > 0) {
            char *tmp = new char[workerM->getAllCountries(signals).length() + 1];
            strcpy(tmp, workerM->getAllCountries(signals).c_str());
            tmp[workerM->getAllCountries(signals).length()] = '\0';
            char *tmp2 = new char[strlen(tmp) + 1];
            strcpy(tmp2, tmp);
            tmp2[strlen(tmp)] = '\0';
            char *country_token = new char[strlen(tmp) + 1];
            country_token = strtok(tmp, "?");
            int child_pos = -1;
            for(int i = 0; i < numWorkers; i++){
                if(childpid[i] == signals){
                    child_pos = i;
                    break;
                }
            }
            if(child_pos < 0) {
                cout << "ERROR finding killed process " << endl;
                exit(5);
            }
            char *killedFD = create_fifo("myfifo", childpid[child_pos]);
            unlink(killedFD);
            killedFD = create_fifo("auxfifo", childpid[child_pos]);
            unlink(killedFD);
            
            childpid[child_pos] = fork();
            if(childpid[child_pos] < 0) {
                cout << "Fork Failed" << endl;
                return errno;
            }
            else if(childpid[child_pos] > 0) {
                myfifo[child_pos] = create_fifo("myfifo", childpid[child_pos]);
                if(mkfifo(myfifo[child_pos], 0666) == -1 && errno != EEXIST) {
                    cout << "Error with main myfifo: " << errno << endl;
                }
                fd[child_pos] = open(myfifo[child_pos], O_WRONLY);

                auxfifo[child_pos] = create_fifo("auxfifo", childpid[child_pos]);
                if(mkfifo(auxfifo[child_pos], 0666) == -1 && errno != EEXIST) {
                    cout << "Error with main auxfifo: " << errno << endl;
                }
                fd2[child_pos] = open(auxfifo[child_pos], O_RDONLY);

            }
            if(childpid[child_pos] == 0) {
                if(execvp("./cmake-build-debug/worker", argumentsv) == -1) {
                    cout << "Exec failed " << endl;
                    return errno;
                }
            }

            int length = strlen(country_token);
            while(true) {
                char *tosend = new char[strlen(cfilepath) + strlen(country_token) + 2];
                sprintf(tosend, "%s/%s", cfilepath, country_token);
                write_line(fd[child_pos], writebuf, bufferSize, tosend);
                workerM->insertSummary(country_token, fd[child_pos], fd2[child_pos], childpid[child_pos]);
                delete[] tosend;

                read_line(fd2[child_pos], readbuf, bufferSize);
                char *c = new char[strlen(readbuf) + 1];
                strcpy(c, readbuf);
                c[strlen(readbuf)] = '\0';
//                print_summary(c);
                delete[] readbuf;
                delete [] tmp;

                tmp = new char[strlen(&tmp2[length+1])+1];
                strcpy(tmp, &tmp2[length+1]);
                tmp[strlen(&tmp2[length+1])] = '\0';
                country_token = strtok(tmp, "?");
                if(!country_token) break;
                length += strlen(country_token)+1;
            }
            write_line(fd[child_pos], writebuf, bufferSize, "OK");
            cout << endl;
            while(true) {
                read_line(fd2[0], readbuf, bufferSize);
                if (strcmp(readbuf, "OK") == 0) break;
            }
            delete [] writebuf;
            delete [] readbuf;
            delete [] tmp2;
            signals = -1;
        }

        cout << "Enter command: " << endl;
        if (!getline(cin, w)) {
            cout<<"w: " << w<<endl;
            cout << "ERROR: " << errno << endl;
            continue;
        }
        stringstream iss(w);
        string word;
        iss >> word;
        if(!w.empty()){
            if(w == "/exit") {
                for(int i = 0; i < numWorkers; i++){
                    char *fifo = create_fifo("myfifo", childpid[i]);
                    close(fd[i]);
                    unlink(fifo);
                    fifo = create_fifo("auxfifo", childpid[i]);
                    close(fd2[i]);
                    unlink(fifo);
                    kill(childpid[i], SIGKILL);
                    cout << "killed child: " << childpid[i] << endl;
                }
                break;
            }
            else if(word == "/topk-AgeRanges") {
                fflush(stdout);
                int identifier;
                char *m = new char[w.length() +1];
                strcpy(m, w.c_str());
                m[w.length()] = '\0';
                string virus, country, date1, date2;
                int k;
                iss >> k;
                cout << "k = " << k << endl;
                if(k > 4) continue;
                iss >> country;
                iss >> virus;
                iss >> date1;
                iss >> date2;
                /*char *m = new char[word.length() + virus.length() + date1.length() + date2.length() + 6];
                sprintf(m, "%s %d %s %s %s", word.c_str(), k, virus.c_str(), date1.c_str(), date2.c_str());*/
                write_line(workerM->writeFD(country), writebuf, bufferSize, m);
                for(int i = 0; i < k; i++){
                    read(workerM->readFD(country), &identifier, sizeof(int));
                    if(identifier < 0) break;
                    cout << "identifier = " << identifier << endl;
                }
            }
            else if(word == "/listCountries" || word == "/searchPatientRecord"){
                int identifier;
                char *m = new char[w.length() +1];
                strcpy(m, w.c_str());
                m[w.length()] = '\0';
                for(int i = 0; i < numWorkers; i++) write_line(fd[i], writebuf, bufferSize, m);
                for(int i = 0; i < numWorkers; i++) {
                    read(fd2[i], &identifier, sizeof(int));
                    if(identifier == -1) continue;
                    else {
                        read_line(fd2[i], readbuf, identifier, bufferSize);
                        cout << readbuf << endl;
                    }
                }
            }
            else if(word == "/numPatientAdmissions" || word == "/numPatientDischarges") {
                char *m = new char[w.length()+1];
                strcpy(m, w.c_str());
                string virus, date1, date2, country, com;
                stringstream iss(w);
                iss >> com;
                iss >> virus;
                iss >> date1;
                iss >> date2;
                iss >> country;
                if(country.empty()) {
//                    int noCountries;
                    /* write command to all processes */
                    for(int i = 0; i < numWorkers; i++) write_line(fd[i], writebuf, bufferSize, m);

                    /* read & print results from all processes */
                    int *noCountries = new int[numWorkers];
                    for(int i = 0; i < numWorkers; i++) read(fd2[i], &noCountries[i], sizeof(int));
                    for(int j = 0; j < numWorkers; j++){
                        for(int i = 0; i < noCountries[j]; i++){
                            read_line(fd2[j], readbuf, bufferSize);
                            cout << readbuf << endl;
                        }
                    }
                }
                else {
                    /* write command to pipe */
                    write_line(workerM->writeFD(country), writebuf, bufferSize, m);

                    /* read & print result */
                    read_line(workerM->readFD(country), readbuf, bufferSize);
                    cout << readbuf << endl;
                }
            }
            else if(word == "/diseaseFrequency"){
                string virus, date1, date2, country;
                int identifier, total = 0;
                iss >> virus;
                iss >> date1;
                iss >> date2;
                iss >> country;
                cout << "country: " << country << endl;
                char *m = new char[w.length() +1];
                strcpy(m, w.c_str());
                m[w.length()] = '\0';
                if(country.empty()) {
                    for(int i = 0; i < numWorkers; i++) write_line(fd[i], writebuf, bufferSize, m);
                    for(int i = 0; i < numWorkers; i++) {
                        read(fd2[i], &identifier, sizeof(int));
                        if(identifier < 0) continue;
                        else total += identifier;
                    }
                    cout << total << endl;
                }
                else {
                    write_line(workerM->writeFD(country), writebuf, bufferSize, m);
                    read_line(workerM->readFD(country), readbuf, bufferSize);
                    cout << readbuf << endl;
                }
            }
            else continue;
        }
        else continue;
    }

    wait(&wstatus);

    return 0;
}

