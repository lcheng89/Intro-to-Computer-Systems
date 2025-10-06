// Author:           Leqian Cheng                                               
// Grade Group:      gg09                                                       
// CS Login:         leqian
#include <stdio.h>

#define ROW 3000
#define COL 500

int arr2D[ROW][COL];

int main() {
    for (int col = 0; col < COL; col++) {
        for (int row = 0; row < ROW; row++) {
            arr2D[row][col] = row + col;
        }
    }
    return 0;
}
