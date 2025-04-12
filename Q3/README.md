# Q3: Distributed Asynchronous Distance Vector Routing

Programming Assignment: [Distributed Asynchronous Distance Vector Routing](https://gaia.cs.umass.edu/kurose_ross/programming/DV/Programming%20Assignment%201.html)

To compile the code, run the following command in the terminal:

1) With no change in link:<br>
    Set `LINKCHANGES = 0` in `distance_vector.c`<br>
    `$ cc distance_vector.c node0.c node1.c node2.c node3.c -o linkchanges.out`<br>
    `$ ./linkchanges.out > no_linkchanges.txt`<br>
    Enter 2 on console and press enter<br><br>
2) With changes in link:<br>
    Set `LINKCHANGES = 1` in `distance_vector.c`<br>
    `$ cc distance_vector.c node0.c node1.c node2.c node3.c -o linkchanges.out`<br>
    `$ ./linkchanges.out > linkchanges.txt`<br>
    Enter 2 on console and press enter

Intermediate and Final outputs analyzed at the bottom of the respective `.txt` files itself.
