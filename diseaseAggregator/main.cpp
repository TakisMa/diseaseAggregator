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

    int wstatus;
    string w;
    pid_t *childpid = new pid_t[numWorkers];
    pid_t parentpid = getpid();
    int dir_count = 0;
    DIR * dirp;
    struct dirent * entry;
    int sent, size;
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
            cout << auxfifo[i] << endl;
            fd2[i] = open(myfifo[i], O_RDONLY);
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
                    workerM->insertSummary(entry->d_name, fd[pos], fd2[pos]);
                    char *tosend = new char[strlen(cfilepath) + strlen(entry->d_name) + 2];
                    sprintf(tosend, "%s/%s", cfilepath, entry->d_name);
                    sent = strlen(tosend);
                    writebuf = new char[sent];
                    strcpy(writebuf, tosend);
                    write(fd[pos], &sent, sizeof(int));
                    cout << "main will send: " << sent << " bytes throught fd: " << fd[0] << endl;
                    write(fd[pos], writebuf, sent);
                    cout << "main will send: " << writebuf << endl;
                    delete[] tosend;
//                    read_line(fd2[pos], readbuf, bufferSize);
//                    cout << readbuf << endl;
                }
            }
        sent = strlen("OK");
        write(fd[0], &sent, sizeof(int));
        write(fd[0], "OK", sent);
        cout << endl;
        closedir(dirp);
    }
    wait(&wstatus);
    return 0;


    while(true) {
        cout << "Enter command: ";
        if (!getline(cin, w)) cout << "exiting" << endl;
        else {
            if(w == "/exit") {
                kill(childpid[0], SIGUSR1);
                break;
            }
            strcpy(writebuf, w.c_str());
            sent = w.length();
            write(fd[0], &sent, sizeof(int));
            while (sent != 0) sent -= write(fd[0], writebuf, w.length());
        }
        int k = read(fd2[0], &size, sizeof(int));
        if (k == -1) cout << "main process read(fd2) errno: " << errno << endl;
        cout << "read size main process: " << size << endl;
        int toread = size;
        while (true) {
            toread -= read(fd2[0], readbuf, k);
            printf("String received in main process: %s\n ", readbuf);
            if ((strcmp(readbuf, "OK") == 0) || toread <= 0) break;
        }
    }

    wait(&wstatus);

    if(close(fd[0]) < 0) {
        cout << "Error on main process close(fd) with code: " << errno << endl;
    }
    if(close(fd2[0]) < 0) {
        cout << "Error on main process close(fd2) with code: " << errno << endl;
    }
    unlink("auxfifo");
    return 0;
}

