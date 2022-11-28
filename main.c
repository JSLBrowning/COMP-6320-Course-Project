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
#include <unistd.h>
RAND_MAX = 32767;


int main() {
   printf("Hello.");
   return 0;
}


double randomNumber() {
   return (double)rand() / (double)RAND_MAX;
}


double randomPoisson(double lambda) {
   // Generate a random number with a Poisson distribution,
   // where lambda is the mean of the distribution.
   double L = exp(-lambda);
   double p = 1.0;
   int k = 0;
   do {
      k++;
      p *= randomNumber();
   } while (p > L);
   return (double)k - 1.0;
}


int randomQueueSelectionSystem(int lambda, int mu) {
   // Since this is just a simulation, queues can just be ints.
   int queue1 = 0;
   int queue2 = 0;


   // Packets arrive following a Poisson process of rate lambda.
   // Packets are assigned to one of the two queues at random.
   int droppedPackets = 0;
   for(int i=0; i<10000; i++) {
      // Wait lambda milliseconds.
      int arrivalMilliseconds = (int)(randomPoisson(lambda) * 1000);
      usleep(arrivalMilliseconds);
      // Generate a random number between 0 and 1.
      double random = randomNumber();
      // If the number is less than 0.5, assign the packet to queue1.
      // Otherwise, assign the packet to queue2.
      if(random < 0.5) {
         if (queue1 < 10) {
            queue1++;
         } else {
            // Packet is dropped.
            droppedPackets++;
         }
      } else {
         if (queue2 < 10) {
            queue2++;
         } else {
            // Packet is dropped.
            droppedPackets++;
         }
      }
   }

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