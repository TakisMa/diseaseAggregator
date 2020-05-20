#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <string>
#include "Functions.h"
#include "FileManagement.h"

using namespace std;

int main(int argc, char *argv[]) {

    /* Check arguments */
    int numWorkers, bufferSize;
    string filePath;
    if(checkArguments(argc, argv, numWorkers, bufferSize, filePath ) != 0) {
        return -10;
    }

    int wstatus;
    pid_t childpid[numWorkers];
    pid_t parentpid = getpid();
    int dir_count = 0;
    DIR * dirp;
    struct dirent * entry;
    int sent, size;
    char *tosent;


    /* Determine number of directories */
    const char *cfilepath = filePath.c_str();
    dirp = opendir(cfilepath);
    if(!dirp) {
        cout << "Failed to open directory" << endl;
        return errno;
    }
    while ((entry = readdir(dirp)) != NULL) {
        if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".")) {
            dir_count++;
        }
    }
    closedir(dirp);


    /* Create/execute children process equal to numWorkers and store pids in childpid array */
    char *pidString =(char * ) malloc(sizeof(parentpid));
    sprintf(pidString, "%d", parentpid);
    char program[]="./worker";
    char *argumentsv[] = {program, NULL};
    for(int i = 0; i < numWorkers; i++) {
        childpid[i] = fork();
        if(childpid[i] < 0) {
            cout << "Fork Failed" << endl;
            return errno;
        }
        if(childpid[i] == 0) {
            if(execvp("./cmake-build-debug/worker", argumentsv) == -1) {
                cout << "Exec failed " << endl;
                return errno;
            }
        }
    }

    for(int i = 0; i < numWorkers; i++) {
        char myfifo[20] = "myfifo_";
        sprintf(pidString, "%d", childpid[i]);
        sprintf(myfifo, "%s%s", myfifo, pidString);
        if(mkfifo(myfifo, 0666) == -1 && errno != EEXIST) {
            cout << "Error with main mkfifo: " << errno << endl;
        }
        cout << myfifo << endl;
    }


    char readbuf[3];
    char writebuf[20];
    int share = dir_count/numWorkers;
    int count = 0;


    if(parentpid == getpid()) {

        char *auxfifo = "auxfifo";
        if(mkfifo(auxfifo, 0666) == -1 && errno != EEXIST) {
            cout << "Error with main auxfifo: " << errno << endl;
        }
        if((dirp = opendir(cfilepath)) != NULL) {
            int fd;
            int fd2 = open(auxfifo, O_RDONLY);
            int pos = 0;
//            while ((entry = readdir(dirp)) != NULL) {
                entry = readdir(dirp);
                char myfifo[20] = "myfifo_";
                if (entry->d_type == DT_DIR && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".")) {
                    sprintf(tosent, "%s/%s", cfilepath, entry->d_name);
                    cout << "to sent string: " << tosent << endl;
                    sprintf(myfifo, "%s%d", myfifo, childpid[pos]);
                    cout << "myfifo: " << myfifo << endl;
                    fd = open(myfifo, O_WRONLY);
                    cout << "Arr: " << entry->d_name << "\t\t Input directory: " << filePath << endl;
                    sent = strlen(tosent);
                    strcpy(writebuf, tosent);
                    write(fd, &sent, sizeof(int));
                    while(sent != 0) {
                        cout << "writebuf will send: " << writebuf << endl;
                        cout << "main process sent is: " << sent << endl;
                        sent -= write(fd, writebuf, strlen(writebuf));
                    }
                    int k = read(fd2, &size, sizeof(int));
                    if(k == -1) {
                        cout << "main process read(fd2) errno: " << errno <<endl;
                    }
                    cout << "read size main process: " << size << endl;
                    int toread = size;
                    while(true) {
                        toread -= read(fd2, readbuf, k);
                        printf("String received in main process: %s\n ", readbuf);
                        if((strcmp(readbuf, "OK") == 0) || toread <= 0) break;
                    }
//                    close(fd);

                    if(share == count) {
                        pos ++;
                        count = 0;
                    }
                    else count ++;
                }
                cout << endl;
//            }


            if(close(fd) < 0) {
                cout << "Error on main process close(fd) with code: " << errno << endl;
            }
            if(close(fd2) < 0) {
                cout << "Error on main process close(fd2) with code: " << errno << endl;
            }
            unlink("auxfifo");
        }
        wait(&wstatus);
        closedir(dirp);
    }
    wait(&wstatus);
    cout << "before return main" << endl;
    return 0;
}
