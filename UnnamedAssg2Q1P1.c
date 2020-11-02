#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

int main(void){
    int fd0[2];
    int fd1[2];
    int fd2[2];
    int fd3[2];
    //fd[0] - read
    //fd[1] - write 

    if (pipe(fd0) == -1){
        printf("Cannot read from pipe, error occured.\n");
        return 1;
    }
    pipe(fd1);
    pipe(fd2);
    pipe(fd3);
    
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
    }

    write(fd0[1], arr, sizeof(float)*5);
    write(fd3[1], arr, sizeof(float)*5);
    close(fd3[1]);
    close(fd0[1]);
    int pid = fork();
    

    if (pid == 0){
        int pid1 = fork();
        if(pid1 == 0){
            //P2
            printf("Entered P2\n");
            int i;
            float arr[5];
            float avg = 0;
            for(i = 0; i < 5; i++){
                read(fd0[0], &arr[i], sizeof(float));
                
                // printf("%f\n",arr[i]);
                avg += arr[i];
            }   
            avg /= 5;
            float x_u = 0;
            for(i = 0; i < 5; i++){
                x_u += (avg - arr[i]) * (avg - arr[i]);
                // printf("%f\n",x_u);
            }
            float sd = sqrt(x_u/5);
            write(fd1[1], &avg, sizeof(float));
            write(fd1[1], &sd, sizeof(float));
            // printf("%f,%f\n",avg,sd);
            close(fd1[1]);
            printf("Leaving P2\n");
        }else{
            sleep(2);
            //P3
            printf("Entered P3\n");
            float arr[5];
            int cat[5];
            int i;
            
            for(i = 0; i < 5; i++){
                read(fd3[0], &arr[i], sizeof(float));
                // printf("%f\n",arr[i]);
            }   
            float avg, sd;
            read(fd1[0],&avg,sizeof(float));
            read(fd1[0],&sd,sizeof(float));
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
                // printf("for L%d, it is CAT%d\n",i+1,cat[i]);
            }
            write(fd2[1], &cat, sizeof(int)*5);
            close(fd2[1]);
            printf("Exiting P3\n");
        }
    }else{
        sleep(4);
        printf("Entering back to P1\n");
        // printf("P1\n");
        //P1 again
        int cat[5];
        int i;
        for (i = 0; i < 5; i++){
            read(fd2[0], &cat[i],sizeof(int));
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
        close(fd3[0]);
        close(fd1[0]);
        close(fd2[0]);
        close(fd0[0]);

    }

    return 0;
}