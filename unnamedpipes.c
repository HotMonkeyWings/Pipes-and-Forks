#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){
    int fd[2];
    //fd[0] - read
    //fd[1] - write 

    if (pipe(fd) == -1){
        printf("Cannot read from pipe, error occured.\n");
        return 1;
    }
    int id = fork();
    if (id == -1){
        printf("Error with forking\n");
        return 4;
    }
    if (id == 0){
        close(fd[0]);
        int x;
        printf("Input number: ");
        scanf("%d", &x);
        printf("Write done at pid: %d and ppid: %d\n",getpid(),getppid());
        if(write(fd[1], &x, sizeof(int)) == -1){
            printf("An error occured while writing to the pipe\n");
            return 2;
        }
        close(fd[1]);
    }else {
        close(fd[1]);
        int y;
        if(read(fd[0], &y, sizeof(int)) == -1){
            printf("An error occured while reading from the pipe\n");
            return 3;
        }
        printf("%d was read from pipe, pid is %d, ppid is %d\n", y, getpid(), getppid());
        close(fd[0]);
    }


    return 0;
}