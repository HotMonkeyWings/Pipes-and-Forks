#include <sys/ipc.h>
#include <string.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define FILENAME "Pass"
#define P1_SEM "P1_stuff"
#define P2_SEM "P2_stuff"
#define P3_SEM "P3_stuff"

#define MAX_LEN 10
#define STRONG 1
#define WEAK 0

typedef struct something
{
    char password[MAX_LEN];
    int alnum_count;
    int special_count;
    int strength;
} pass;

int main(void)
{
    //Setting up the shared memory
    key_t key = ftok(FILENAME, 0);
    int shmid = shmget(key, sizeof(pass), 0666 | IPC_CREAT);
    pass *data = (pass *)shmat(shmid, (void *)0, 0);

    //Unlinked semophores just incase
    sem_unlink(P1_SEM);
    sem_unlink(P2_SEM);
    sem_unlink(P3_SEM);

    //Create new semaphores
    sem_t *p1semo = sem_open(P1_SEM, O_CREAT, 0666, 0);
    sem_t *p2semo = sem_open(P2_SEM, O_CREAT, 0666, 0);
    sem_t *p3semo = sem_open(P3_SEM, O_CREAT, 0666, 0);

    if (p1semo == SEM_FAILED)
    {
        printf("Error: Failed to open Semaphore of P1");
    }

    if (p2semo == SEM_FAILED)
    {
        printf("Error: Failed to open Semaphore of P2");
    }

    if (p3semo == SEM_FAILED)
    {
        printf("Error: Failed to open Semaphore of P3");
    }
    // printf("Reached here");
    //Time to fork
    int pid = fork();
    if (pid == 0)
    {
        int ppid = fork();
        if (ppid == 0)
        {
            //P2
            sem_wait(p2semo);
            // printf("P2");
            int i = 0;
            int alnum_count = 0;
            for (i = 0; i < strlen(data->password); i++)
            {
                if (('a' <= data->password[i] && data->password[i] <= 'z') || ('A' <= data->password[i] && data->password[i] <= 'Z') || ('0' <= data->password[i] && data->password[i] <= '9'))
                {
                    alnum_count++;
                }
            }
            data->alnum_count = alnum_count;
            data->special_count = i - alnum_count;
            // printf("%d %d",data->alnum_count, data->special_count);
            sem_post(p3semo);
            // printf("Ended P2\n");
            sem_close(p2semo);
        }
        else
        {
            //P3
            // printf("P3 waiting\n");
            sem_wait(p3semo);
            // printf("Reached P3\n");
            if (data->alnum_count >= data->special_count)
            {
                data->strength = WEAK;
            }
            else
            {
                data->strength = STRONG;
            }
            sem_post(p1semo);
            sem_close(p3semo);
        }
    }
    else
    {
        //P1

        printf("Enter password: ");
        scanf("%s", data->password);
        sem_post(p2semo);

        sem_wait(p1semo);

        if (data->strength == WEAK)
        {
            printf("The password is weak.\n");
        }
        else
        {
            printf("The password is strong.\n");
        }
        sem_close(p1semo);
        sem_unlink(P1_SEM);
        sem_unlink(P2_SEM);
        sem_unlink(P3_SEM);
    }
    return 0;
}