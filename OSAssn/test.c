#include <stdio.h>
#include <linux/kernel.h>

#include <sys/syscall.h>
#include <unistd.h>

#define sys_kernel_2d_memcpy 448
int main(){
	float mat1[5][5];
	float mat2[5][5];
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			mat1[i][j]=i*j;
		}
	}
	long res=syscall(sys_kernel_2d_memcpy,5,5,(float*)mat1, (float*)mat2);
	printf("\nPrinting the original matrix: \n");
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			printf("%f ",mat1[i][j]);
		}
		printf("\n");
	}
	printf("\nPrinting the copied matrix: \n");
	for(int i=0;i<5;i++){
		for(int j=0;j<5;j++){
			printf("%f ",mat2[i][j]);
		}
		printf("\n");
	}
	
	return 0;
}

