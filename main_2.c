/* ================================== *
 * File Name: main_2.c                *
 * Author: Austin Preston             *
 * ================================== */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define MAX_BUFFER_SIZE 10 // Max limit of buffer
#define MAX_PACKETS 10000 // Max number of packets to simulate
#define NUM_THREADS  3 // Number of threads

/* Global variables */

int buffer_1[MAX_BUFFER_SIZE];
int buffer_2[MAX_BUFFER_SIZE];

int front_1 = 0, front_2 = -1;
int rear_1 = 0, rear_2 = -1;

int buffer_1_length = 0;
int buffer_2_length = 0;

int simulated_packets = 0;
int arrived_packets = 0;

pthread_cond_t assign_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_1_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_2_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t assign_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t buffer_1_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t buffer_2_lock = PTHREAD_MUTEX_INITIALIZER;

/* Function declaration */

void *Packet_Assignment(); /* Inserts a packet in the FIFO queue. */
void *Server_1_Departure(); /* Deletes a packet from the FIFO queue 1. */
void *Server_2_Departure(); /* Delete packet from the FIFO queue 2. */
int Random_Number(); /* Chooses a random number between 1 and 2. */
void Print_Queue_1(); /* Prints out the elements in Queue 1; For debugging */
void Print_Queue_2(); /* Prints out the elements in Queue 2; For debugging */

int main(int argc, char *argv[]) {
  int thread_created;
  int i = 1;
  double lambda, mu;
  pthread_t threads[NUM_THREADS];

  /* Check that program arguments are correct */
  if (argc != 3) {
    fprintf(stderr, "Invalid arguments. Usage: ./<out> <lamba> <mu>\n");
    exit(-1);
  }

  for (; i < 3; i++) {
    if (!isdigit(*argv[i])) {
      fprintf(stderr, "Arguments (lamba, mu) must be an integer!\n");
      exit(-1);
    }
  }

  /* Set lambda and mu from input */
  lambda = atof(argv[1]);
  mu = atof(argv[2]);

  /* Create threads for packet assignment and departure */
  thread_created = pthread_create(&threads[0], NULL, Packet_Assignment, (void *) 0);
  if (thread_created) { fprintf(stderr, "Unable to create thread: %d\n", 0); exit(-1); }
  thread_created = pthread_create(&threads[1], NULL, Server_1_Departure, (void *) &mu);
  if (thread_created) { fprintf(stderr, "Unable to create thread: %d\n", 1); exit(-1); }
  thread_created = pthread_create(&threads[2], NULL, Server_2_Departure, (void *) &mu);
  if (thread_created) { fprintf(stderr, "Unable to create thread: %d\n", 2); exit(-1); }

  /* Begin packet simulation */
  while (simulated_packets < (MAX_PACKETS - 1)) {
    /* Create packet at rate lambda */
    sleep(lambda);
    simulated_packets++;

    printf("A new packet has arrived!\n");

    /* Signal Packet Assignment thread if waiting */
    pthread_mutex_lock(&assign_lock);
    arrived_packets++;
    if (arrived_packets == 1) {
      pthread_cond_signal(&assign_cond);
    }
    pthread_mutex_unlock(&assign_lock);
  }

  /* Wait for threads to finish before terminating */
  (void) pthread_join(threads[0], NULL);
  (void) pthread_join(threads[1], NULL);
  (void) pthread_join(threads[2], NULL);

  return 0;
}

void *Packet_Assignment(void *thread_id) {
  int rand_num;
  int blocked_packets = 0, packet_num = 0;

  pthread_mutex_lock(&assign_lock);
  while (simulated_packets < (MAX_PACKETS - 1) || arrived_packets != 0) {

    /* Make the thread wait if there are no packets to assign */
    while (arrived_packets == 0) {
      pthread_cond_wait(&assign_cond, &assign_lock);
    }
    arrived_packets--;
    pthread_mutex_unlock(&assign_lock);

    /* Assign a packet to a random queue */
    rand_num = Random_Number();

    if (rand_num == 1) { /* Append to queue 1 */
        printf("Packet assigned to queue 1\n");

      /* Check if queue 1 is full */
      pthread_mutex_lock(&buffer_1_lock);
      if (buffer_1_length == (MAX_BUFFER_SIZE - 1 )) {
        printf("Packet %d blocked in queue 1\n", packet_num);
        blocked_packets++;
        pthread_mutex_unlock(&buffer_1_lock);
      }
      /* Queue not full; Add packet */
      else {
        pthread_mutex_unlock(&buffer_1_lock);
        rear_1++;
        if (rear_1 == (MAX_BUFFER_SIZE - 1)) {
          rear_1 = 0;
        }
        buffer_1[rear_1] = simulated_packets;

        /* Debugging */
        printf("After insertion - ");
        Print_Queue_1();
        /* Increment queue 1 length */
        pthread_mutex_lock(&buffer_1_lock);
        buffer_1_length++;
        if (buffer_1_length == 1) {
          pthread_cond_signal(&buffer_1_cond);
        }
        pthread_mutex_unlock(&buffer_1_lock);
      }
    }
    else { /* Append to queue 2 */
      printf("Packet assigned to queue 2\n");

      /* Check if queue 2 is full */
      pthread_mutex_lock(&buffer_2_lock);
      if (buffer_2_length == (MAX_BUFFER_SIZE - 1)) {
        printf("Packet %d blocked in queue 2\n", packet_num);
        pthread_mutex_unlock(&buffer_2_lock);
      }

      /* Queue not full; Add packet */
      else {
        pthread_mutex_unlock(&buffer_2_lock);
        if (buffer_2_length == (MAX_BUFFER_SIZE - 1)) {
        printf("Packet %d blocked in queue 2\n", packet_num);
        pthread_mutex_unlock(&buffer_2_lock);
      }

      /* Queue not full; Add packet */
      else {
        pthread_mutex_unlock(&buffer_2_lock);
        rear_2++;
        if (rear_2 == (MAX_BUFFER_SIZE - 1)) {
            rear_2 = 0;
        }
        buffer_2[rear_2] = simulated_packets;

        /* Debugging */
        printf("After insertion - ");
        Print_Queue_2();

        /* Increment queue 2 length */
        pthread_mutex_lock(&buffer_2_lock);
        buffer_2_length++;
        if (buffer_2_length == 1) {
          pthread_cond_signal(&buffer_2_cond);
        }
        pthread_mutex_unlock(&buffer_2_lock);
      }
    }
    packet_num++;
    pthread_mutex_lock(&assign_lock);
  }
  printf("Total blocked packets: %d\n", blocked_packets);
}

void *Server_1_Departure(void *arg) {
  double mu = *((double *) arg);

  pthread_mutex_lock(&buffer_1_lock);
  while (simulated_packets != (MAX_PACKETS - 1) || buffer_1_length != 0) {

    /* Queue is empty; Wait */
    while (buffer_1_length == 0) {
      pthread_cond_wait(&buffer_1_cond, &buffer_1_lock);
    }
    pthread_mutex_unlock(&buffer_1_lock);

    /* Queue is not empty; Remove first packet in queue */
    sleep(1.0/mu); // Time needed for a packet to depart server
    front_1++;
    if (front_1 == (MAX_BUFFER_SIZE - 1)) {
      front_1 = 0;
    }

    /* Decrement queue length */
    pthread_mutex_lock(&buffer_1_lock);
    buffer_1_length--;
    pthread_mutex_unlock(&buffer_1_lock);

    /* Error checking; Queue 1 is out of synchronization if true */
    //    if ( (front_1 > rear_1) {
    //  fprintf(stderr, "Err: Queue 1 not in proper synchronization.\n");
    //  exit(-1);
    //      }

    /* Debugging */
    printf("After deletion - ");
    Print_Queue_1();

    pthread_mutex_lock(&buffer_1_lock);
  }
}

void *Server_2_Departure(void *arg) {
  double mu = *((double *) arg);

  pthread_mutex_lock(&buffer_2_lock);
  while (simulated_packets != (MAX_PACKETS - 1) || buffer_2_length != 0) {

    /* Queue is empty; Wait */
    while (buffer_2_length == 0) {
      pthread_cond_wait(&buffer_2_cond, &buffer_2_lock);
    }
    pthread_mutex_unlock(&buffer_2_lock);

    /* Queue is not empty; Remove first packet in queue */
    sleep(1.0/mu); // Time needed for a packet to depart server
    front_2++;
    if (front_2 == (MAX_BUFFER_SIZE - 1)) {
      front_2 = 0;
    }

    /* Decrement queue length */
    pthread_mutex_lock(&buffer_2_lock);
    buffer_2_length--;
    pthread_mutex_unlock(&buffer_2_lock);

    /* Error checking; Queue 2 is out of synchronization if true */
    //    if (front_2 > rear_2) {
    //      fprintf(stderr, "Err: Queue 2 not in proper synchronization.\n");
    //      exit(-1);
    //    }

    /* Debugging */
    printf("After deletion - ");
    Print_Queue_2();

    pthread_mutex_lock(&buffer_2_lock);
  }
}

int Random_Number() {
  int num, lower = 1, upper = 2;

  srand(time(0));
  num = (rand() % (upper - lower + 1)) + lower;

  return num;
}

void Print_Queue_1() {
  int i;

  if (front_1 == -1) {
    printf("Queue 1 is empty\n");
  }
  else {
    printf("Queue 1: ");
    for (i = front_1; i <= rear_1; i++) {
      printf("%d ", buffer_1[i]);
    }
    printf("\n");
  }
}

void Print_Queue_2() {
  int i;

  if (front_2 == -1) {
    printf("Queue 2 is empty");
  }
  else {
    printf("Queue 2: ");
    for (i = front_2; i <= rear_2; i++) {
      printf("%d ", buffer_2[i]);
    }
    }
    printf("\n");
  }
}