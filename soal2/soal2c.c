#include <stdlib.h>
#include <unistd.h>
int pid;
int pipe1[2];
int pipe2[2];
void main() {
  pipe(pipe1);
  pid = fork();
if (pid == 0)
	exec1();
 pipe(pipe2);
pid = fork();
 if (pid == 0)
	exec2();
  close(pipe1[0]);
  close(pipe1[1]);
pid = fork();
  if (pid == 0)
	exec3();
}

void exec3() {
  dup2(pipe2[0], 0);
  close(pipe2[0]);
  close(pipe2[1]);
  execlp("head", "head","-5", NULL);
}
void exec2() {

  dup2(pipe1[0], 0);
  dup2(pipe2[1], 1);
  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);
  execlp("sort","sort","-nrk","3,3", NULL);
}
void exec1() {
  dup2(pipe1[1], 1);
  close(pipe1[0]);
  close(pipe1[1]);
  execlp("ps", "ps", "aux",NULL);
}
