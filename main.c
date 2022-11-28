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


double randomExponential(double mu) {
   // Generate a random number with an exponential distribution,
   // where mu is the mean of the distribution.
   return -log(randomNumber()) / mu;
}


int randomQueueSelectionSystem(int lambda, int mu) {
   // Since this is just a simulation, queues can just be ints.
   int queue1 = 0;
   int queue2 = 0;
   int arrivalCountdown = 0;
   int queueOneDepartureCountdown = 0;
   int queueTwoDepartureCountdown = 0;
   int packetsArrived = 0;
   int droppedPackets = 0;


   while( packetsArrived < 10000 && queue1 > 0 && queue2 > 0 ) {
      if (arrivalCountdown == 0) {
         // Generate a random number with a Poisson distribution,
         // where lambda is the mean of the distribution.
         arrivalCountdown = randomPoisson(lambda);
         if (queue1 < 10 && queue2 < 10) {
            // If both queues are less than 10, randomly select one.
            if (randomNumber() < 0.5) {
               queue1++;
            } else {
               queue2++;
            }
         } else if (queue1 < 10 && queue2 >= 10) {
            queue1++;
            packetsArrived++;
         } else if (queue2 < 10 && queue1 >= 10) {
            queue2++;
            packetsArrived++;
         } else {
            droppedPackets++;
            packetsArrived++;
         }
      }
      if (queueOneDepartureCountdown == 0) {
         if (queue1 > 0) {
            queue1--;
            queueOneDepartureCountdown = randomExponential(mu);
         }
      }
      if (queueTwoDepartureCountdown == 0) {
         if (queue2 > 0) {
            queue2--;
            queueTwoDepartureCountdown = randomExponential(mu);
         }
      }
      arrivalCountdown--;
      queueOneDepartureCountdown--;
      queueTwoDepartureCountdown--;
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