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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

double randomNumber()
{
  // Generate a random number between 0 and 1 with a uniform distribution.
  return (double)rand() / (double)RAND_MAX;
}

int randomPoisson(int lambda)
{
  // Generate a random number with a Poisson distribution,
  // where lambda is the mean of the distribution.

  double L = exp(-lambda);
  double p = 1.0;
  int k = 0;

  // Keep recomputing if k = 0.
  while ((k - 1) < 1)
  {
    k = 0, p = 1.0;

    do
    {
      k++;
      p *= randomNumber();
    } while (p > L);

    printf("%d\n", k - 1);
  }

  return k - 1;
}

int randomQueueSelectionSystem(int lambda, int mu)
{
  // Since this is just a simulation, queues can just be ints.
  int queue1 = 0;
  int queue2 = 0;

  int arrivalCountdown = randomPoisson(lambda);
  int queueCountdown = randomPoisson(mu);

  int packetsArrived = 0;
  int droppedPackets = 0;

  int arrivalTimer = 0;
  int queue1Timer = -1;
  int queue2Timer = -1;

  // Debugging; Inspect the arrival and queue countdown timers.
  printf("Arrival Countdown is: %d\n", arrivalCountdown);
  printf("Queue Countdown is: %d\n", queueCountdown);

  while (packetsArrived < 10000 || (queue1 > 0 || queue2 > 0))
  {
    // Debugging; Inspect the queue during the loop.
    //    printf("Packet Number: %d\n", packetsArrived);
    //    printf("Queue 1: %d\n", queue1);
    //    printf("Queue 1 Timer: %d\n", queue1Timer);
    //    printf("Queue 2: %d\n", queue2);
    //    printf("Queue 2 Timer: %d\n", queue2Timer);

    if (arrivalTimer == 0 && packetsArrived < 10000)
    {
      packetsArrived++;

      // Regenerate the arrival countdown.
      arrivalTimer = arrivalCountdown;

      // Send the new arrival to the queues.
      if (queue1 < 10 && queue2 < 10)
      {
        // If both queues are less than 10, randomly select one.
        if (randomNumber() < 0.5)
        {
          if (queue1 != 0)
          {
            queue1++;
          }
          else
          {
            queue1++;
            queue1Timer = queueCountdown;
          }
        }
        else
        {
          if (queue2 != 0)
          {
            queue2++;
          }
          else
          {
            queue2++;
            queue2Timer = queueCountdown;
          }
        }
      }
      else if (queue1 < 10 && queue2 == 10)
      {
        queue1++;
      }
      else if (queue2 < 10 && queue1 == 10)
      {
        queue2++;
      }
      else
      {
        droppedPackets++;
      }
    }

    if (queue1Timer == 0)
    {
      if (queue1 > 0)
      {
        queue1--;
      }
      queue1Timer = queueCountdown;
    }

    if (queue2Timer == 0)
    {
      if (queue2 > 0)
      {
        queue2--;
      }
      queue2Timer = queueCountdown;
    }

    arrivalTimer--;
    queue1Timer--;
    queue2Timer--;

    // Debugging; Watch the simulation at a slower pace
    //    sleep(1);
  }

  printf("DROPPED PACKETS: %d\n", droppedPackets);
  int successfulPackets = packetsArrived - droppedPackets;
  printf("PACKETS ARRIVED: %d\n", successfulPackets);
  return successfulPackets;
}

int minQueueSelectionSystem(int lambda, int mu)
{
  // Since this is just a simulation, queues can just be ints.
  int queue1 = 0;
  int queue2 = 0;
  int arrivalCountdown = 0;
  int queueOneDepartureCountdown = 0;
  int queueTwoDepartureCountdown = 0;
  int packetsArrived = 0;
  int droppedPackets = 0;

  while (packetsArrived < 10000 || (queue1 > 0 && queue2 > 0))
  {
    if (arrivalCountdown == 0 && packetsArrived < 10000)
    {
      // Send the new arrival to the queues.
      if (queue1 < 10 && queue2 < 10)
      {
        if (queue1 < queue2)
        {
          queue1++;
          packetsArrived++;
        }
        else if (queue2 < queue1)
        {
          queue2++;
          packetsArrived++;
        }
        else
        {
          // If the queues are equal, randomly select one.
          if (randomNumber() < 0.5)
          {
            queue1++;
            packetsArrived++;
          }
          else
          {
            queue2++;
            packetsArrived++;
          }
        }
      }
      else if (queue1 < 10 && queue2 == 10)
      {
        queue1++;
        packetsArrived++;
      }
      else if (queue2 < 10 && queue1 == 10)
      {
        queue2++;
        packetsArrived++;
      }
      else
      {
        droppedPackets++;
        packetsArrived++;
      }

      // Regenerate the arrival countdown.
      arrivalCountdown = randomPoisson(lambda);
    }

    if (queueOneDepartureCountdown == 0)
    {
      if (queue1 > 0)
      {
        queue1--;
      }
      queueOneDepartureCountdown = randomPoisson(mu);
    }

    if (queueTwoDepartureCountdown == 0)
    {
      if (queue2 > 0)
      {
        queue2--;
      }
      queueTwoDepartureCountdown = randomPoisson(mu);
    }

    arrivalCountdown--;
    queueOneDepartureCountdown--;
    queueTwoDepartureCountdown--;
  }

  printf("DROPPED PACKETS: %d\n", droppedPackets);
  int successfulPackets = packetsArrived - droppedPackets;
  printf("PACKETS ARRIVED: %d\n", successfulPackets);
  return successfulPackets;
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    printf("Invalid parameters.\n");
    printf("Usage: ./main <choice> <lambda> <mu>\n");
    printf("Choice: 1 for random queue selection, 2 for minimum queue selection.\n");
    return 1;
  }
  else
  {
    int choice = atoi(argv[1]);
    int lambda = atoi(argv[2]);
    int mu = atoi(argv[3]);

    // Seed random number
    srand(time(0));

    if (choice == 1)
    {
      randomQueueSelectionSystem(lambda, mu);
    }
    else if (choice == 2)
    {
      minQueueSelectionSystem(lambda, mu);
    }
    else
    {
      printf("Invalid parameters.\n");
      printf("Usage: ./main <choice> <lambda> <mu>\n");
      printf("Choice: 1 for random queue selection, 2 for minimum queue selection.\n");
    }

    return 0;
  }
}
