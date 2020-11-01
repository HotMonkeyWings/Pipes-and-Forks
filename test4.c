#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    int arr[5];
    int fd = open("sum", O_RDONLY);
    if (fd == -1){
        return 1;
    }


    int i,ans = 0;
    for (i = 0; i < 5; i++){
        read(fd, &arr[i], sizeof(arr[i]));
        printf("Read %d\n", arr[i]);
        ans += arr[i];
    }
    printf("%d\n",ans);
    close(fd);
    fd = open("sum", O_WRONLY);
    write(fd, &ans, sizeof(int));
    printf("Answer written\n");
    close(fd);
}