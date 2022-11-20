/*H*
 * FILENAME: main.c
 * 
 * DESCRIPTION:
 *       Simulate a two-queue system, where each queue in the system
 *       has a capacity of 10 packets (including the one in the server).
 * 
 * AUTHORS:
 *       James S. L. Browning and Austin David Preston
 *H*/

#include <stdio.h>

int main() {
   printf("Hello.");
   return 0;
}


// Create two queue data structures, one for each queue in the system.
// Each queue will "process" packets with a service rate of mu.

// Create a packet data structure.
// Packets will be sent to the system with an arrival rate of lambda.