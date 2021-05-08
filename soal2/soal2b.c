#include<stdio.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
pthread_t tid[25];
long long int matrixnew[6][6];
int a,b,ii=0,jj=0;
void* fact(void *arg){
int loca=a,locb=b,locii=ii,locjj=jj;
	long long int hasil=loca;
	if(loca!=0&&locb!=0){
	if(loca>=locb)
    	{
    	int j=1;
    	for(int i=loca-1;j<locb;j++,i--)
        	{
        	hasil=hasil*i;
        	}
    	matrixnew[locii][locjj]=hasil;
    	}
	else if(locb>loca)
    	{
    	for(int i=loca-1;i>0;i--)
        	{
        	hasil=hasil*i;
        	}
    	matrixnew[locii][locjj]=hasil;
    	}
	}
	else matrixnew[locii][locjj]=0;

}

int main(int argc,char *argv[])
{
	key_t key=1234;
	int *shmatrix;
	int segid=shmget(key,sizeof(int)*4*6,IPC_CREAT|0666);
	shmatrix=(int *)shmat(segid,NULL,0);
	int inmatrix[30];
    
	printf("shared matrix\n");
	for(int i=0;i<4;i++){
    	for(int j=0;j<6;j++){
            	printf("%d\t",shmatrix[i*6+j]);
    	}
    	printf("\n");
    	}
	printf("input matriks (col=6,row=4)\n");
	for(int i=0;i<4;i++)
       	for(int j=0;j<6;j++)
           	scanf("%d",&inmatrix[i*6+j]);

 	int count=0;
	while(count<25)
	{
	a=shmatrix[ii*6+jj];
	b=inmatrix[ii*6+jj];
	pthread_create(&(tid[count]),NULL,&fact,NULL);
	usleep(500);
	if(jj==5)
    	{
    	ii++;
    	jj=0;
    	}
	else
    	jj++;

	count++;
	}

	for(int i=0;i<25;i++)
    	pthread_join(tid[i],NULL);

	printf("hasil\n");
	for(int i=0;i<4;i++){
    	for(int j=0;j<6;j++){
        	printf("%lld\t\t",matrixnew[i][j]);
    	}
    	printf("\n");
	}
	shmdt(shmatrix);
	shmctl(segid,IPC_RMID,NULL);
}
