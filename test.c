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
#include <stdlib.h>
#include <math.h>


double randomNumber() {
    // Generate a random number between 0 and 1 with a uniform distribution.
    return (double)rand() / (double)RAND_MAX;
}


int randomPoisson(int lambda) {
    // Generate a random number with a Poisson distribution,
    // where lambda is the mean of the distribution.
    double L = exp(-lambda);
    double p = 1.0;
    int k = 0;
    do {
       k++;
       p *= randomNumber();
    } while (p > L);
    return k - 1;
}


double randomExponential(double mu) {
    // Generate a random number with an exponential distribution,
    // where mu is the mean of the distribution.
    return -log(randomNumber()) / mu;
}


int main() {
    printf("=== RANDOM NUMBER TEST. ===\n");
    printf("=  VALUE BETWEEN 0 AND 1. =\n");
    for(int i = 0; i < 10; i++) {
        double newPackets = randomNumber();
        printf("%f\n", newPackets);
    }

    printf("= POISSON TEST. =\n");
    printf("= LAMBDA OF 10. =\n");
    for(int i = 0; i < 10; i++) {
        int newPackets = randomPoisson(10);
        printf("%d\n", newPackets);
    }

    printf("=== RANDOM NUMBER TEST. ===\n");
    printf("=  VALUE BETWEEN 0 AND 1. =\n");
    for(int i = 0; i < 10; i++) {
        double newPackets = randomExponential(.5);
        printf("%f\n", newPackets);
    }

    return 0;
}