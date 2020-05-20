#ifndef DISEASEAGGREGATOR_FUNCTIONS_H
#define DISEASEAGGREGATOR_FUNCTIONS_H


#include "Record.h"

using namespace std;

bool check_int(string );
int checkArguments(int argc, char** argv, int &workersNum, int &bufferSize, string &path);
void swapD(struct Date *a, struct Date *b);
int partition(struct Date *array[], int low, int high);
void quickS(struct Date *array[], int low, int high);

int read_line(int fd, char *readbuf);
int initialize_record(char *filepath, char *country, Record *record);

static int compare_dates(const void *p, const void *q);

#endif //DISEASEAGGREGATOR_FUNCTIONS_H
