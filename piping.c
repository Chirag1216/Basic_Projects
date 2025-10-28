#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>


int main(){

  int id = fork();
  if(id == 0){
    int fd = open("buffer.txt",O_WRONLY);

    dup2(fd,1);
    close(fd);
    execl("/usr/bin/ls","ls","-a",NULL);
  }
  else{
    wait(NULL);
    execlp("grep","grep","hello","buffer.txt",NULL);
  }
  return 0;
}
