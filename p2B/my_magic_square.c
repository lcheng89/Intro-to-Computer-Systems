////////////////////////////////////////////////////////////////////////////////
// Main File:        my_magic_square.c
// This File:        my_magic_square.c
// Other Files:      magic_squareN.txt
// Semester:         CS 354 Lecture 001 Spring 2025
// Grade Group:      gg09  (Canvas course -> People -> click on your name to see your gg#)
// Instructor:       Mahmood
// 
// Author:           Leqian Cheng
// Email:            lcheng89@wisc.edu
// CS Login:         leqian
//
// add your own File Header information here (as described in Commenting guide)

///////////////////////////////////////////////////////////////////////////////
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2025, Hina Mahmood
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
	int size;           // dimension of the square
	int **magic_square; // ptr to 2D heap array that stores magic square values
} MagicSquare;

/* TODO:
 * Prompts the user for magic square's size, read size, and
 * check if it is an odd number >= 3 
 * If not valid size, display the required error message and exit
 *
 * return the valid number
 */
int getSize() {
	int n;
	printf("Enter magic square's size (odd number >= 3):\n");
	scanf("%d", &n);
	if (n % 2 == 0) {
		printf("Magic square size must be odd.\n"); 
        exit(1);
    }
    if (n < 3) {
		printf("Magic square size must be >= 3.\n"); 
        exit(1);
    }
	return n;
} 

/* TODO:
 * Creates a magic square of size n on the heap
 *
 * May use the Siamese magic square algorithm or alternative
 * algorithm that produces a valid magic square 
 *
 * n - the number of rows and columns
 *
 * returns a pointer to the completed MagicSquare struct
 */
MagicSquare *generateMagicSquare(int n) {
	// Allocate memory for MagicSquare struct
	MagicSquare *ms = (MagicSquare *)malloc(sizeof(MagicSquare));
	if (!ms) {
        perror("Failed to allocate memory for MagicSquare");
        exit(1);
    }
    ms->size = n;

	// Allocate memory for 2D array
    ms->magic_square = (int **)malloc(n * sizeof(int *));
    if (!ms->magic_square) {
        perror("Failed to allocate memory for magic square rows");
        exit(1);
    }
	
	for (int i = 0; i < n; i++) {
		*(ms->magic_square + i) = (int *)malloc(n * sizeof(int));
		if (!*(ms->magic_square + i)) {
			perror("Failed to allocate memory for magic square columns");
			exit(1);
		}
	}

	// Initialize magic square with zeros
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            *(*(ms->magic_square + i) + j) = 0;
        }
    }

    // Siamese method to generate magic square
    int num = 1;
    int row = 0;
    int col = n / 2;

    while (num <= n * n) {
        *(*(ms->magic_square + row) + col) = num++;
        int newRow = (row - 1 + n) % n;
        int newCol = (col + 1) % n;

        if (*(*(ms->magic_square + newRow) + newCol) != 0) {
            row = (row + 1) % n;  // Move down if occupied
        } else {
            row = newRow;
            col = newCol;
        }
    }

    return ms;
}

/* TODO:  
 * Open a new file (or overwrite the existing file)
 * and write magic square values to the file
 * in a format specified in the assignment.
 *
 * See assignment for required file format.
 *
 * magic_square - the magic square to write to a file
 * filename - the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {
	FILE *fp = fopen(filename, "w");
	if(!fp) {
		perror("Failed to open the file");
		exit(1);
	}

	int n = magic_square->size;
	fprintf(fp, "%d\n", n);
	
	// Compute width based on the maximum number in the square (n*n) to align
	int max = n * n;
	int width = 0;
	while (max > 0) {
		width++;
		max /= 10;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			fprintf(fp, "%*d" ,width, *(*(magic_square->magic_square+i)+j));
			if (j != n-1) {
				fprintf(fp, ",");
			}
		}
		fprintf(fp, "\n");
	}

	if (fclose(fp) != 0) {
		perror("Failed to close the file");
		exit(1);
	}

}


/* TODO:
 * Calls other functions to generate a magic square 
 * of the user-specified size and outputs the
 * created square to the output filename.
 * 
 * Add description of required CLAs here
 */
int main(int argc, char **argv) {
	// TODO: Check input arguments to get output filename
	if(argc != 2) {
		printf("Usage: ./my_magic_square <output_filename>\n");
		exit(1);
	}

	// TODO: Get magic square's size from user
	int n = getSize();

	// TODO: Generate a magic square by correctly interpreting 
	//       the algorithm(s) in the write-up or by writing on your own.  
	//       You must confirm that your program produces a 
	//       valid Magic Square. See the provided Wikipedia page link for
    //       description.
	MagicSquare *magic_square = generateMagicSquare(n);

	// TODO: Output the magic square
	fileOutputMagicSquare(magic_square, *(argv+1));

	// Free all heap memory
	for (int i = 0; i < n; i++) {
		free(*(magic_square->magic_square+i));
	}
	free(magic_square->magic_square);
	free(magic_square);

	return 0;
} 

// 202501


