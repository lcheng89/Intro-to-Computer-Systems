// Author:           Leqian Cheng                                               
// Grade Group:      gg09                                                       
// CS Login:         leqian
#include <stdio.h>

#define ROW 128
#define COL 8

int arr2D[ROW][COL];

int main(){
	for(int iteration=0; iteration<100; iteration++){
		for(int row=0; row<ROW; row+=64){
			for(int col=0; col<COL; col++){
				arr2D[row][col] = iteration + row + col;
			}
		}
	}
	return 0;
}
		
