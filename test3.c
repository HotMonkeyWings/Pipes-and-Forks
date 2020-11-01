#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    int arr[5];
    int fd = open("sum", O_WRONLY);
    if (fd == -1){
        return 1;
    }


    int i;
    for (i = 0; i < 5; i++){
        arr[i] = 123+i*10/3;
        printf("Generated %d\n",arr[i]);
    }
    write(fd, arr, sizeof(int)*5);
    
    close(fd);
    fd = open("sum", O_RDONLY);
    int ans;
    read(fd, &ans, sizeof(int));
    printf("Answer Received: %d\n", ans);
    close(fd);
}