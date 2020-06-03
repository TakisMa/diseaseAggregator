#include "SignalHandling.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

using namespace std;
int signals;

void kill_child(int signum, siginfo_t *info, void *context) {
    signals = SIGINT;
//    kill(getpid(), SIGKILL);
}

void new_file(int signum, siginfo_t *info, void *context) {
    signals = SIGUSR1;
}

void child_int(int signum, siginfo_t *info, void *context) {
    int wstatus;
    signals = info->si_pid;
    waitpid(info->si_pid, &wstatus, WNOHANG);
}
