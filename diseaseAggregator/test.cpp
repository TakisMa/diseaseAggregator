#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>
#include <fstream>
#include "Record.h"

using namespace std;

int main(int argc, char *argv[]) {
    cout << "First Arg: " << argv[1] << endl;
    char myfifo[20] = "myfifo_";
    char pid[6];
    sprintf(pid, "%d", getpid());
    sprintf(myfifo, "%s%s", myfifo, pid);
    printf("fifo in child process: %s\n", myfifo);
    char *auxfifo = "auxfifo";
    DIR * dirp;
    struct dirent * entry;
    FILE *fp;
    Record *record = new Record();


    /* Create named piped for read & write */
    if(mkfifo(myfifo, 0666) == -1 && errno != EEXIST) {
        cout << "Error with mkfifo of child proc: " << errno << endl;
    }
    else cout << "pipe: " << myfifo << " created successfully" << endl;
    if(mkfifo(auxfifo, 0666) == -1 && errno != EEXIST) {
        cout << "Error with main auxfifo: " << errno << endl;
    }
    int fd = open(myfifo, O_RDONLY | O_NONBLOCK);
    int fd2 = open(auxfifo, O_WRONLY);

    /* Reading from main process */
    char arr[20];
    char country[20];
    char filepath[100];
    char filename[10];
    size_t lenght = 0;
    char *line = NULL;
    while(true) {
        strcpy(filepath, "input/");
        read(fd, arr, sizeof(arr));
        strcpy(country, arr);
        sprintf(filepath, "%s%s",filepath, arr);
        if(strcmp(arr, "OK") == 0) break;
        else{
            dirp = opendir(filepath);
            if(!dirp) {
                cout << "Failed to open directory" << endl;
//                return errno;
            }
            while ((entry = readdir(dirp)) != NULL) {
                if (entry->d_type == DT_REG && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".")) {
                    cout << "tmp: " << country << endl;
                    sprintf(filename, "%s/%s", filepath, entry->d_name);
                    cout << "tmp: " << country << endl;
                    cout << "Filename is: " << filename << endl;
                    fp = fopen(filename, "r");
                    while(getline(&line, &lenght, fp) != -1 ) {
                        if(record->initialize(line, entry->d_name, country)) cout << "Country = " << record->getCountry() << endl;
                        else cout << "Failed to initialize record " << endl;

                    }
                }

            }
        }
        cout << "String received in child process: " << arr << endl;
//        printf("String received in child process: %s\n", arr);
        write(fd2, "OK", strlen("OK"));
    }

    /* Closing and removing named pipes */
    if(close(fd) < 0) {
        cout << "Error on child process close(fd) with code: " << errno << endl;
    }
    if(close(fd2) < 0) {
        cout << "Error on child process close(fd2) with code: " << errno << endl;
    }
    unlink(myfifo);
}