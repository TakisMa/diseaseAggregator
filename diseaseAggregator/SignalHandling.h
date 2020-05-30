#ifndef DISEASEAGGREGATOR_SIGNALHANDLING_H
#define DISEASEAGGREGATOR_SIGNALHANDLING_H
#include <signal.h>
extern int signals;

void kill_child(int signum, siginfo_t *info, void *context);
void new_file(int signum, siginfo_t *info, void *context);
void child_int(int signum, siginfo_t *infom, void *context);

#endif //DISEASEAGGREGATOR_SIGNALHANDLING_H
