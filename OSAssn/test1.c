#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int helper(int rows,int col, float* mat1, float* mat2){
    float temp[col];
	int i;
	for(i=0;i<rows;i++){
		memcpy(temp,mat1+i*col,sizeof(float)*col);
        memcpy(mat2+i*col,temp,sizeof(float)*col);
		
	}
	printf("Copied from one matrix to another\n");
	return 0;
}
int main(){
	float mat1[5][5];
	float mat2[5][5];
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			mat1[i][j]=i*j;
		}
	}
	helper(5,5,(float*)mat1,(float*)mat2);
    for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			printf("%f ",mat2[i][j]);
		}
		printf("\n");
	}
	
	return 0;
}