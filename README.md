# cop4520-assignment2

To run code: 

gcc/g++ <filename> 
./a.out


Problem 1:
This problem is a version of the N prisoners problem. The algorithm that the guests
must utilize requires that a guest be designated as the leader. The leader can effectively
count the number of guests by incrementing a global counter whenever they revisit the party
room with the cupcake. The cupcake mimics the light in the prisoner problem, as it will indicate
whether a new guest has ate a cupcake when it was there in the period that the leader was gone. 
The guests know whether to take or leave the cupcake based on if they've already had one. Once
incrementing their count, only the leader can replace a cupcake (request from the servants).
  
The code runs well under a second. T: 0.006112 ms

Problem 2: 
Choice 2) The Minotaur’s second strategy allowed the guests to place a sign on the door 
indicating when the showroom is available. The sign would read “AVAILABLE” or “BUSY.” Every guest 
is responsible to set the sign to “BUSY” when entering the showroom and back to “AVAILABLE” upon exit. 
That way guests would not bother trying to go to the showroom if it is not available.

A: Of the three choices this is my ideal strategy because it doesn't require that guests simply wait at
the door in a queue or have the thread become inactive/off/sleeping when it's waiting. Downsides include 
the fact that we may be performing unneccesary checks, and the frequency of guests being able to get in 
the room multiple times is greater, since they don't have to wait in a line. 
