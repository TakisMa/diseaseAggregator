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
    child_act.sa_flags = SA_SIGINFO | SA_RESTART;
    child_act.sa_sigaction = child_int;
//    child_act.sa_flags = SA_NOCLDWAIT;
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
                }
            }
            write_line(fd[pos], writebuf, bufferSize, "OK");
            cout << endl;
            closedir(dirp);
    }
    while(true) {
        read_line(fd2[0], readbuf, bufferSize);
        if (strcmp(readbuf, "OK") == 0) break;
    }


    cout << "childpid: " << childpid[0] << endl;
    while(true) {
        if(signals > 0) {
            cout << "Child with pid: " << signals << " died..." << endl;
            cout << "countries for dead worker: " << workerM->getAllCountries(signals) << endl;
            char *tmp = new char[workerM->getAllCountries(signals).length() + 1];
            strcpy(tmp, workerM->getAllCountries(signals).c_str());
            tmp[workerM->getAllCountries(signals).length()] = '\0';
            char *tmp2 = new char[strlen(tmp) + 1];
            strcpy(tmp2, tmp);
            tmp2[strlen(tmp)] = '\0';
            char *country_token = new char[strlen(tmp) + 1];
            country_token = strtok(tmp, "?");
//            printf("tmp2: %s\n", &tmp2[strlen(tmp)+1]);
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
            signals = -1;
        }

        if (!getline(cin, w)) {
            cout<<"w: " << w<<endl;
            cout << "ERROR: " << errno << endl;
            continue;
        }
        else if(!w.empty()){
            if(w == "/exit") {
                for(int i = 0; i < numWorkers; i++){
                    char *fifo = create_fifo("myfifo", childpid[i]);
                    unlink(fifo);
                    fifo = create_fifo("auxfifo", childpid[i]);
                    unlink(fifo);
                    kill(childpid[i], SIGKILL);
                }
                break;
            }
            else if(w == "/listCountries"){
                char *m = new char[w.length() +1];
                strcpy(m, w.c_str());
                m[w.length()] = '\0';
                for(int i = 0; i < numWorkers; i++) {
                    write_line(fd[i], writebuf, bufferSize, m);
                }
            }
            else {
                char *m = new char[w.length() +1];
                strcpy(m, w.c_str());
                m[w.length()] = '\0';
                write_line(fd[0], writebuf, bufferSize, m);
            }
        }
        else continue;
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

