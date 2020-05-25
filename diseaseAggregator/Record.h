#ifndef DISEASEMONITOR_RECORD_H
#define DISEASEMONITOR_RECORD_H

#include <string>

using namespace std;

struct Date {
    int day;
    int month;
    int year;
    string date;
    int compare(Date *date) {
        if(date->year > year) return -1;
        else if(date->year < year) return 1;
        else if(date->month > month) return -1;
        else if(date->month < month) return 1;
        else if(date->day > day) return -1;
        else if(date->day < day) return 1;
        else return 0;
    }
};

class Record {
private:
    int recordID;
    int age;
    string state;
    string firstName;
    string lastName;
    string diseaseID;
    string country;
    Date entryDate;
    Date exitDate;
public:
    void setState(const string &state);

    int getAge() const;

    const string &getState() const;

int getRecordId() const;

    const string &getFirstName() const {
        return firstName;
    }

    const string &getLastName() const {
        return lastName;
    }

    const string &getDiseaseId() const;

    const string &getCountry() const;

    Date *getEntryDate();

    Date *getExitDate();

    void setExitDate(Date *exitDate);

    bool isDischarged();

    bool initialize(string str, string date, string country);

    Record();

    ~Record();
};


#endif //DISEASEMONITOR_RECORD_H
