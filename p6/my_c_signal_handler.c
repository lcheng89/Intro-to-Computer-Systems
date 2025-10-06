////////////////////////////////////////////////////////////////////////////////
// Main File:        my_c_signal_handler.c
// This File:        my_c_signal_handler.c
// Other Files:      send_signal.c,	my_div0_handler.c
// Semester:         CS 354 Lecture 001 Spring 2025
// Grade Group:      gg09  (Canvas course -> People -> click on your name to see your gg#)
// Instructor:       Mahmood
// 
// Author:           Leqian Cheng
// Email:            lcheng89@wisc.edu
// CS Login:         leqian
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

static int interval = 3;           // alarm interval in seconds
static int usr1_count = 0;         // count of SIGUSR1 received

void handle_alarm(int signo) {
    pid_t pid = getpid();
    time_t now = time(NULL);
    char *timestr = ctime(&now);
    if (timestr) {
        // ctime() returns a string ending in '\n', strip it
        timestr[strcspn(timestr, "\n")] = '\0';
    }
    printf("PID: %d CURRENT TIME: %s\n", pid, timestr);
    fflush(stdout);
    alarm(interval);
}

void handle_usr1(int signo) {
    usr1_count++;
    printf("Received SIGUSR1, user signal 1 counted.\n");
    fflush(stdout);
}

void handle_sigint(int signo) {
    printf("SIGINT handled.\n");
    printf("SIGUSR1 was handled %d times. Exiting now.\n", usr1_count);
    fflush(stdout);
    exit(0);
}

int main(void) {
    struct sigaction sa;

    // Setup SIGALRM handler
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_alarm;
    if (sigaction(SIGALRM, &sa, NULL) < 0) {
        perror("sigaction(SIGALRM)");
        return EXIT_FAILURE;
    }

    // Setup SIGUSR1 handler
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_usr1;
    if (sigaction(SIGUSR1, &sa, NULL) < 0) {
        perror("sigaction(SIGUSR1)");
        return EXIT_FAILURE;
    }

    // Setup SIGINT handler
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;
    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction(SIGINT)");
        return EXIT_FAILURE;
    }

    // Start the periodic alarm
    alarm(interval);

    printf("PID and current time: prints every %d seconds.\n", interval);
    printf("Type Ctrl-C to end the program.\n");
    fflush(stdout);

    // Infinite loop waiting for signals
    while (1) {
        pause();
    }

    return 0;
}


