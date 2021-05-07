#include<stdio.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include <unistd.h>
int main()
{
  int y1=4,x1=3,y2=3,x2=6;
  int mat1[6][6];//input mat1
  int mat2[6][6];//input mat2
  int *matrix;
  int segid;
  key_t key=1234;
  segid=shmget(key,sizeof(int)*x2*y1,IPC_CREAT|0666);
  matrix = (int *)shmat(segid,NULL,0);

	printf("input matriks 1 (col=3,row=4)\n");
	for(int i=0;i<y1;i++)
    	for(int j=0;j<x1;j++)
        	scanf("%d",&mat1[i][j]);

	printf("input matriks 2 (col=6,row=3)\n");
	for(int i=0;i<y2;i++)
    	for(int j=0;j<x2;j++)
        	scanf("%d",&mat2[i][j]);


	for(int i=0;i<y1;i++)
    	for(int j=0;j<x2;j++){
        	matrix[i*x2+j]=0;
        	for(int k=0;k<x1;k++)
            	matrix[i*x2+j]+=mat1[i][k]*mat2[k][j];
        	}
	printf("matriks hasil perkalian\n");
	for(int i=0;i<y1;i++){
    for(int j=0;j<x2;j++){
   	 printf("%d\t",matrix[i*x2+j]);
   	 }
    printf("\n");
    }

  execlp("./b","./b",NULL);
  shmdt(matrix);
  shmctl(segid,IPC_RMID,NULL);

}
