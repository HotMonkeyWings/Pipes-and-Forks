#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <linux/stat.h>
#include <fcntl.h>
#include <string.h>


int main(void){
    // int fd0[2];
    // int fd1[2];
    // int fd2[2];
    // int fd3[2];
    //fd[0] - read
    //fd[1] - write 
    umask(0);
    mknod("P1_stuff", S_IFIFO|0666, 0);
    umask(0);
    mknod("P2_stuff", S_IFIFO|0666, 0);
    umask(0);
    mknod("P3_stuff", S_IFIFO|0666, 0);


    // if (pipe(fd0) == -1){
    //     printf("Cannot read from pipe, error occured.\n");
    //     return 1;
    // }
    // pipe(fd1);
    // pipe(fd2);
    // pipe(fd3);
    
    // int fd1 = open("P1_stuff", O_RDWR | O_CREAT | O_TRUNC, 0777);
    // int fd2 = open("P1_stuff", O_RDWR | O_CREAT | O_TRUNC, 0777);
    // int fd3 = open("P13_stuff", O_RDWR | O_CREAT | O_TRUNC, 0777);
    
    

    // write(fd0, arr, sizeof(float)*5);
    // write(fd3, arr, sizeof(float)*5);
    // close(fd3);
    
    // float arr2[5];
    // int trial = open("P1_stuff", O_RDONLY, 0);
    // for(i = 0; i < 5; i++){
    //     read(fd0, &arr2[i], sizeof(float));
                
    //     printf("%f\n",arr2[i]);
    // }



    int pid = fork();
    
    

    if (pid == 0){
        int pid1 = fork();
        if(pid1 == 0){
            //P2
            // printf("Entered P2\n");
            int i;
            float arr[5];
            float avg = 0;
            // printf("Test1");
            int fd0 = open("P1_stuff", O_RDONLY, 0);
            // printf("%d",fd0);
            for(i = 0; i < 5; i++){
                read(fd0, &arr[i], sizeof(float));
                
                // printf("%f\n",arr[i]);
                avg += arr[i];
            }   
            close(fd0);
            
            avg /= 5;
            float x_u = 0;
            for(i = 0; i < 5; i++){
                x_u += (avg - arr[i]) * (avg - arr[i]);
                // printf("%f\n",x_u);
            }
            float sd = sqrt(x_u/5);
            int fd1 = open("P2_stuff", O_WRONLY, 0);
            write(fd1, &avg, sizeof(float));
            write(fd1, &sd, sizeof(float));
            // printf("%f,%f\n",avg,sd);
            close(fd1);
            // printf("Leaving P2\n");
        }else{
            // sleep(2);
            // wait(NULL);
            //P3
            // printf("Entered P3\n");
            float arr[5];
            int cat[5];
            int i;
            int fd0 = open("P1_stuff", O_RDONLY);
        
            for(i = 0; i < 5; i++){
                read(fd0, &arr[i], sizeof(float));
                // printf("%f\n",arr[i]);
            }   
            close(fd0);
            float avg, sd;
            int fd1 = open("P2_stuff", O_RDONLY);
            read(fd1,&avg,sizeof(float));
            read(fd1,&sd,sizeof(float));
            close(fd1);
            int fd2 = open("P3_stuff", O_WRONLY);
            for(i = 0; i < 5; i++){
                if (arr[i] == avg){
                    cat[i] = 0;
                }
                else if(arr[i] > avg + sd){
                    cat[i] = 1;
                }
                else if(arr[i] > avg){
                    cat[i] = 2;
                }
                else if(arr[i] > avg-sd){
                    cat[i] = 3;
                }
                else if(arr[i] < avg-sd){
                    cat[i] = 4;
                }
                write(fd2, &cat[i], sizeof(int));
                // printf("for L%d, it is CAT%d\n",i+1,cat[i]);
            }
            close(fd2);
            
            
            // printf("Exiting P3\n");
        }
    }else{
        int fd0 = open("P1_stuff", O_RDWR, 0);
        //P1
        int i;
        float arr[5];
        for(i = 0; i < 5;i++){
            printf("Thermostat reading for L%d: ",i+1);
            scanf("%f",&arr[i]);
            if (arr[i] < 15 || arr[i] > 45){
                printf("Invalid reading\n");
                i--;
            }
            else{
                write(fd0, &arr[i],sizeof(float));
            }
        }
        close(fd0);
        // sleep(4);
        // wait(NULL);
        // printf("Entering back to P1\n");
        // printf("P1\n");
        //P1 again
        int cat[5];
        int fd2 = open("P3_stuff", O_RDONLY);
        for (i = 0; i < 5; i++){
            // printf("%f\n",arr[i]);
            read(fd2, &cat[i],sizeof(int));
            // printf("%d\n",cat[i]);
            if(cat[i] == 0){
                continue;
            }
            else if(cat[i] == 1){
                arr[i] -= 3;
            }
            else if(cat[i] == 2){
                arr[i] -= 1.5f;
            }
            else if(cat[i] == 3){
                arr[i] += 2;
            }
            else if(cat[i] == 4){
                arr[i] += 2.5f;
            }
            printf("New L%d:%f\n",i+1,arr[i]);
        }
        // close(fd3[0]);
        // close(fd1[0]);
        // close(fd2[0]);
        // close(fd0[0]);
        close(fd2);

    }

    return 0;
}