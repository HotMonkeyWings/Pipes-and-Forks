//Unnamed pipelines and Named pipelines

There are 2 fork() functions that are called.

The second fork is P2, the first fork is P3 and the main program is P1.

3 Pipes were defined, along with 1 to act as a temporary one.

In P1, 
First, it reads the array of temperatures from the user and stores it in the first and temporary pipe.
Then it waits for the P3 to complete categorising,
afterwards printing the details.

In P2,
The array is read from the first pipe, and the average and standard deviation are calculated.
This is then stored in the second pipe.

In P3,
The array is read from the temporary pipe, the average and standard deviation are read from the second pipes.
It then categorises the array values which are stored in the third pipe.

Both pipelines (unnamed and named) uses the same algorithm. The only difference being is that wait(NULL) was used in unnamed pipelines, while the pipe simply waits to be opened for reading and writing in the named implementation.
