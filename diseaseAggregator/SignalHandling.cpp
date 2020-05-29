#include "SignalHandling.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

using namespace std;
int signals;

void kill_child(int signum, siginfo_t *info, void *context) {
    signals = signum;
    /*cout << "signals: " << signals << endl;
    cout << "Child pid: " << getpid() << endl;*/
    kill(getpid(), SIGKILL);
}

void child_int(int signum, siginfo_t *info, void *context) {
    int wstatus;
//    cout << "signal caught ";
    signals = info->si_pid;
    waitpid(info->si_pid, &wstatus, WNOHANG);
//    cout << signals << endl;
}
