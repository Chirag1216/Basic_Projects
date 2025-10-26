#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>  


int main(){
  int n;
  printf("Enter the number of elements in the array: ");
  scanf("%d",&n);

  int arr[n];

  for(int j = 0;j<n;j++){
    printf("Enter the %dth element : ",j);
    scanf("%d",&arr[j]);
  }

  int start,stop,step = 2,sum = 0;
  int size = sizeof(arr)/sizeof(int);
  int fd[2];

  if(pipe(fd) == -1){
    printf("The pipe failed\n");
    return 1;
  }

  int id1 = fork();
  int id2 = fork();
 
  if(id1 == 0 && id2 == 0){
    kill(getpid(),SIGKILL);
  }


  if(id1 == 0){
    if(id2!=0){
      start = 1;
      stop = size/2;
    }
  }else if(id2 == 0){
    if((size/2)%2 == 0){
      start = size/2 + 1;
    }else{
      start = size/2;
    }
    stop = size;
  }else{
    start = 0;
    stop = size;
  }
  
  for(int i = start;i<stop;i +=step){
    sum = sum + arr[i];
  }

  if(id1 == 0){
    if(id2 !=0){
      close(fd[0]);
      printf("sum from 1st child %d\n",sum);
      write(fd[1],&sum,sizeof(int));
      close(fd[1]);
    }
  }else if(id2 == 0){
    close(fd[0]);
    printf("sum from 2nd child %d\n",sum);
    write(fd[1],&sum,sizeof(int));
    close(fd[1]);
  }else{
    wait(NULL);
    wait(NULL);
    int sum1,sum2;
    close(fd[1]);
    read(fd[0],&sum1,sizeof(int));
    read(fd[0],&sum2,sizeof(int));
    close(fd[0]);
    printf("Parent process sum is %d\n",sum);
    printf("The total sum is %d\n",sum1+sum2+sum);
  }

  return 0;
}
