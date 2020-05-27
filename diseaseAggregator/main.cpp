#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <string>
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
            cout << fd2[i] << endl;
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
                    char *tosend = new char[strlen(cfilepath) + strlen(entry->d_name) + 2];
                    sprintf(tosend, "%s/%s", cfilepath, entry->d_name);
                    write_line(fd[pos], writebuf, bufferSize, tosend);
                    delete[] tosend;

                    read_line(fd2[pos], readbuf, bufferSize);
                    char *c = new char[strlen(readbuf) + 1];
                    strcpy(c, readbuf);
                    c[strlen(readbuf)] = '\0';
                    print_summary(c);
                    delete[] readbuf;
                    tmp = new char[strlen(entry->d_name) + 1];
                    strcpy(tmp, entry->d_name);
                    tmp[strlen(entry->d_name)] = '\0';
                    cout << tmp << endl;
                }
            }
            workerM->insertSummary(tmp, fd[pos], fd2[pos], childpid[pos]);
            write_line(fd[pos], writebuf, bufferSize, "OK");
            cout << endl;
            closedir(dirp);
    }
    cout << workerM->writeFD("China") << endl;
    while(true) {
        read_line(fd2[0], readbuf, bufferSize);
        if (strcmp(readbuf, "OK") == 0) break;
    }
    cout << childpid[0] << endl;
    while(true) {
        if(signals > 0) {
            cout << "Child with pid: " << signals << " died..." << endl;
            cout << "countries for dead worker: " << workerM->getAllCountries(signals) << endl;
            signals = -1;
        }

        if (!getline(cin, w)) continue;
        else {
            if(w == "/exit") {
                kill(childpid[0], SIGUSR1);
                break;
            }
            char *m = new char[w.length() +1];
            strcpy(m, w.c_str());
            m[w.length()] = '\0';
            write_line(fd[0], writebuf, bufferSize, m);
        }
    }

    wait(&wstatus);

    if(close(fd[0]) < 0) {
        cout << "Error on main process close(fd) with code: " << errno << endl;
    }
    if(close(fd2[0]) < 0) {
        cout << "Error on main process close(fd2) with code: " << errno << endl;
    }
    return 0;
}

