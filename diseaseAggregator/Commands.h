#ifndef DISEASEAGGREGATOR_COMMANDS_H
#define DISEASEAGGREGATOR_COMMANDS_H


#include "Hashtable.h"
#include "RecordIDManagement.h"

int diseaseFrequency(string virusName, string date1, string date2, string country, Hashtable *diseaseHT, int fd2);

int listCountries(char *filepath);

void numPatientAdmissions(string virusName, string date1, string date2, string country, Hashtable *diseaseHT, char *filepath);

void numPatientDischarges(string virusName, string date1, string date2, string country, Hashtable *diseaseHT, char *filepath);

int select_command(Hashtable *diseaseHT, Hashtable* countryHT, ID_Hashtable *idHT, char *filepath, string w, int fd2);



#endif //DISEASEAGGREGATOR_COMMANDS_H
