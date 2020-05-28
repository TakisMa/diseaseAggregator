#include "SignalHandling.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std;
int signals;

void kill_child(int signum, siginfo_t *info, void *context) {
    signals = signum;
    cout << "signals: " << signals << endl;
    kill(getpid(), SIGKILL);
}

void child_int(int signum, siginfo_t *info, void *context) {
    cout << "signal caught" << endl;
    signals = info->si_pid;
}
