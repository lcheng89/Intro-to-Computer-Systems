////////////////////////////////////////////////////////////////////////////////                    
// Main File:        send_signal.c                                      
// This File:        send_signal.c                                     
// Other Files:      my_c_signal_handler.c, my_div0_handler.c                            
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
#include <string.h>
#include <errno.h>
#include <libgen.h> 

int main(int argc, char *argv[]) {
    if (argc != 3) {
		const char *prog = basename(argv[0]);
        fprintf(stderr,
                "Usage: %s -u <pid> to send SIGUSR1\n"
                "       %s -i <pid> to send SIGINT\n",
                prog, prog);
        return EXIT_FAILURE;
    }

    int sig;
    if (strcmp(argv[1], "-u") == 0) {
        sig = SIGUSR1;
    } else if (strcmp(argv[1], "-i") == 0) {
        sig = SIGINT;
    } else {
        fprintf(stderr, "Unknown option '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    char *endptr;
    pid_t pid = (pid_t)strtol(argv[2], &endptr, 10);
    if (*endptr != '\0' || pid <= 0) {
        fprintf(stderr, "Invalid pid '%s'\n", argv[2]);
        return EXIT_FAILURE;
    }

    if (kill((pid_t)pid, sig) < 0) {
        fprintf(stderr, "Failed to send signal: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    return 0;
}

