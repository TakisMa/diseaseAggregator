#ifndef DISEASEAGGREGATOR_FUNCTIONS_H
#define DISEASEAGGREGATOR_FUNCTIONS_H

#include "Record.h"
#include "Hashtable.h"
#include "RecordIDManagement.h"

using namespace std;



bool check_int(string );
int checkArguments(int argc, char** argv, int &workersNum, int &bufferSize, string &path);
void swapD(struct Date *&a, struct Date *&b);
int partition(struct Date *array[], int low, int high);
void quickS(struct Date *array[], int low, int high);

int read_line(int fd, char *&readbuf, int bufferSize);
int read_line(int fd, char *&readbuf, int size, int bufferSize);
void write_line(int fd, char *&writebuf, int bufferSize, char *message);
int initialize_record(char *filepath, char *country, Hashtable *diseaseHT, Hashtable *countryHT, ID_Hashtable *idHT, int fd2, int bufferSize);
int sort_files(char* filepath, Date **&file_array);
char *create_fifo(char *fifo_name, pid_t childpid);
void print_summary(char *summary);

static int compare_dates(const void *p, const void *q);

#endif //DISEASEAGGREGATOR_FUNCTIONS_H
