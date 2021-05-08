#include <stdlib.h>
#include <unistd.h>
int pid;
int pipe1[2];
int pipe2[2];

void three(){
  dup2(pipe2[0], 0);
  close(pipe2[0]);
  close(pipe2[1]);
  execlp("head", "head","-5", NULL);
}
void two(){
  dup2(pipe1[0], 0);
  dup2(pipe2[1], 1);
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  execlp("sort","sort","-nrk","3,3", NULL);
}
void one(){
  dup2(pipe1[1], 1);
  close(pipe1[0]);
  close(pipe1[1]);
  execlp("ps", "ps", "aux",NULL);
}
int main(){
  pipe(pipe1);
  pipe(pipe2);
 if (fork()==0)
	one();
 if (fork()==0)
	two();
  close(pipe1[0]);
  close(pipe1[1]);
  if (fork()==0)
	three();
}
