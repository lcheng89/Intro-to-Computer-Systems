#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: cla_sequence start_integer end_integer where start is less than end\n");
        return 1;
    }

    // Convert arguments to integers using strtol
    char *endptr1, *endptr2;
    int start = strtol(argv[1], &endptr1, 10);
    int end = strtol(argv[2], &endptr2, 10);

    // Validate conversion (ensure entire string was consumed)
    if (*endptr1 != '\0' || *endptr2 != '\0') {
        printf("Usage: cla_sequence start_integer end_integer where start is less than end\n");
        return 1;
    }

    // Check if start is less than end
    if (start >= end) {
        printf("Usage: cla_sequence start_integer end_integer where start is less than end\n");
        return 1;
    }

    for (int i = start; i <= end; i++) {
        printf("%d", i);
        if (i < end) {
            printf(",");
        }
    }
    printf("\n");

    return 0;
}
