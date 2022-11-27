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


int randomQueueSelectionSystem(int lambda, int mu) {
   // Since this is just a simulation, queues can just be ints.
   int queue1 = 0;
   int queue2 = 0;

   // Packets arrive following a Poisson process of rate lambda.
   // Packets are assigned to one of the two queues at random.

   // If the queue is full (one packet being processed and nine in waiting), the new packet is dropped.
   // Packets are processed at an exponentially distributed rate with mean 1/mu.
}


int minQueueSelectionSystem(int lambda, int mu) {
   // Packets arrive following a Poisson process of rate lambda.
   // Packets are assigned to whichever queue has the fewest packets in waiting.
      // Random queue if both are the same length.
   // If the queue is full (one packet being processed and nine in waiting), the new packet is dropped.
   // Packets are processed at an exponentially distributed rate with mean 1/mu.
}