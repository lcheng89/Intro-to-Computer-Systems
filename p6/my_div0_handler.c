////////////////////////////////////////////////////////////////////////////////                    
// Main File:        my_div0_handler.c                                 
// This File:        my_div0_handler.c                                    
// Other Files:      my_c_signal_handler.c, send_signal.c                             
// Semester:         CS 354 Lecture 001 Spring 2025                             
// Grade Group:      gg09  (Canvas course -> People -> click on your name to see your gg#)
// Instructor:       Mahmood                                                    
//                                                                              
// Author:           Leqian Cheng                                               
// Email:            lcheng89@wisc.edu                                          
// CS Login:         leqian                                                     
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

static int op_count = 0;

void handle_fpe(int signo) {
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", op_count);
    printf("The program will be terminated.\n");
    fflush(stdout);
    exit(0);
}

void handle_sigint(int signo) {
	printf("\n");
    printf("Total number of operations completed successfully: %d\n", op_count);
    printf("The program will be terminated.\n");
    fflush(stdout);
    exit(0);
}

int main(void) {
    struct sigaction sa;
    char buf[100];
    long a, b;

    // SIGFPE handler
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_fpe;
    if (sigaction(SIGFPE, &sa, NULL) < 0) {
        perror("sigaction(SIGFPE)");
        return EXIT_FAILURE;
    }

    // SIGINT handler
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;
    if (sigaction(SIGINT, &sa, NULL) < 0) {
        perror("sigaction(SIGINT)");
        return EXIT_FAILURE;
    }

    while (1) {
        printf("Enter first integer: ");
        if (!fgets(buf, sizeof(buf), stdin)) break;
        a = strtol(buf, NULL, 10);

        printf("Enter second integer: ");
        if (!fgets(buf, sizeof(buf), stdin)) break;
        b = strtol(buf, NULL, 10);

        long q = a / b;         // may raise SIGFPE
        long r = a % b;
        op_count++;
        printf("%ld / %ld is %ld with a remainder of %ld\n", a, b, q, r);
        fflush(stdout);
    }

    return 0;
}

