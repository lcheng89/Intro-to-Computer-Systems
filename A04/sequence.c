#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Check for the correct number of command-line arguments.
    if (argc != 5) {
        printf("Usage: ./sequence n x0 m c   ==> where next value in sequence is computed as x1 = m * x0 + c\n");
        printf("where: n is an non-zero positive number of values in the sequence,\n");
        printf("       x0 is an integer and is the first value in the sequence,\n");
        printf("       m is an integer and is used as a multiplier of the previous term in the sequence,\n");
        printf("       c is an integer and is added to the (m*previous) term\n");
        return 1; // Return non-zero if error occurs.
    }

    // Convert arguments to integers.
    int n = atoi(argv[1]);
    int x = atoi(argv[2]);
    int m = atoi(argv[3]);
    int c = atoi(argv[4]);

    // Ensure that n is a positive number.
    if(n <= 0) {
        printf("Error: n must be a non-zero positive number.\n");
        return 1;
    }

    // Generate and print the sequence.
    for (int i = 0; i < n; i++) {
        printf("%d", x);
        if (i < n - 1) {  // Print comma after every term except the last.
            printf(",");
        }
        x = m * x + c;  // Compute the next term.
    }
    printf("\n");

    return 0; // Return zero to indicate success.
}
