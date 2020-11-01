#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void){
    // int fd[2], nbytes;
    // pid_t childpid;
    // char string[] = "Hello world!\n";
    // char readbuffer[80];

    // pipe(fd);
    // if((childpid=fork())== -1){
    //     perror("fork");
    //     exit(1)
    // }
    fork();
    pid_t process_id;
    pid_t p_process_id;

    process_id = getpid();
    p_process_id = getppid();

    

    printf("The process id is %d \n",process_id);
    printf("The parent process id is %d \n",p_process_id);

    return 0;
}