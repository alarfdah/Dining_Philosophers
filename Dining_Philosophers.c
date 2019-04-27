/**
 * Date: 04/29/2019
 * Class: CS5541
 * Assignment: Assignment 5 - Dining_Philosophers
 * Author: Ahmed Radwan
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>


#define PHILOSOPHERS 5
#define FORKS 5

void *philosopher_process() {

}

int main(int argc, char **argv) {
	// Declaring Variables
	int i;

	// Create a thread for each philosopher
	pthread_t philosopher[PHILOSOPHERS];

	// Create semaphores/'forks'
	sem_t *fork[FORKS];

	for (i = 0; i < PHILOSOPHERS; i++) {
		// Malloc for each semaphore
		fork[i] = malloc(sizeof(sem_t));

		// Initialize the semaphores
		// 1 is the number of threads that
		// can access the sem at a given time
		sem_init(fork[i], 0, 1);	
	}

	// Create threads
	for (i = 0; i < PHILOSOPHERS; i++) {
		pthread_create(&(philosopher[i]), NULL, philosopher_process, NULL);
	}

	for (i = 0; i < PHILOSOPHERS; i++) {
		pthread_join(philosopher[i], NULL);
	}

	for (i = 0; i < PHILOSOPHERS; i++) {
		sem_destroy(fork[i]);
	}

	return 0;
}
