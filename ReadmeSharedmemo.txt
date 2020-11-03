//Shared memory

First, we define some prerequisites and also a struct to store the password.

The shared memory is setup with ftok, shmget and shmat.
The password structure is stored here.

All the semophores are unlinked just incase, and then are created.

After this two forks are called.
The second fork is for P2 while the first fork is for P3.
The main program is for P1.


In P1,
The password is read from the user.
sem_post is called for P2 semograph to start P2.
sem_wait is called on P1 semograph to pause P1 till P3 is done checking the password strength.

Afterwards, the strength is simply printed and all the semographs are unlinked.

In P2,
The sem_wait here ensures P2 only starts after sem_post is called on P2 semograph.
Once it is called, it reads the number of alpha numeric characters in the password.
Assuming the rest would be symbols, the number of alpha numeric characters are reduced from the total to find the number of special characters.
These are stored onto the struct in the shared memory.
The sem_post to P3 semograph starts process P3.

In P3,
The sem_wait here ensures P3 only starts after sem_post is called on P3 semograph.
Once it is called, it simply compares the count of alpha numeric characters and special characters.
It stores the result in the struct present in the shared memory.
It then executes the sem_post command to P1 semograph to go back to process P1 to print the results.

