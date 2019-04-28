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
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include "args.h"

#define PHILOSOPHERS 5


int get_forks(sem_t *fork_left, sem_t *fork_right, int index) {

	struct timespec time;
	int timedwaitResult = 0;
	int count = 0;
	int done = 0;

	do {
			clock_gettime(CLOCK_REALTIME, &time);
			time.tv_sec += 1;

			// Grab fork to the left
			sem_wait(fork_left);
			printf("Philosopher %d grabbed the LEFT fork\n", index);

			// Wait for other Philosophers to grab the left fork
			sleep(1);

			// Try to grab the fork to the right
			timedwaitResult = sem_timedwait(fork_right, &time);
			printf("Philosopher %d is trying to grab the RIGHT fork\n", index);

			// If I couldn't get the fork to the right
			if (timedwaitResult == -1) {
				printf("Philosopher %d couldn't grab the RIGHT fork... Releasing LEFT fork.\n", index);
				sem_post(fork_left);
				sleep(rand() % 5);
				count++;
			} else {
				printf("Philosopher %d grabbed BOTH the forks\n", index);
				done = 1;
			}

	} while (!done);

	return count;
}

void *philosopher_process(void * myargs) {
	arguments *args = (arguments *)myargs;
	int deadlock_count = 0;
	// int i = 0;
	int index = args->index;

	sem_t *fork_left;
	sem_t *fork_right;

	// Assign forks
	switch (index) {
		case 0:
			fork_left = args->fork_sem->fork_one;
			fork_right = args->fork_sem->fork_two;
		break;
		case 1:
			fork_left = args->fork_sem->fork_two;
			fork_right = args->fork_sem->fork_three;
		break;
		case 2:
			fork_left = args->fork_sem->fork_three;
			fork_right = args->fork_sem->fork_four;
		break;
		case 3:
			fork_left = args->fork_sem->fork_four;
			fork_right = args->fork_sem->fork_five;
		break;
		case 4:
			fork_left = args->fork_sem->fork_five;
			fork_right = args->fork_sem->fork_one;
		break;
		default:
			printf("Error, wrong index %d\n", index);
	}

	deadlock_count += get_forks(fork_left, fork_right, args->index);
	printf("DONE: Philosopher %d had %d deadlocks.\n", args->index, deadlock_count);
	sem_post(fork_left);
	sem_post(fork_right);
	sem_close(fork_left);
	sem_close(fork_right);

	pthread_exit(NULL);
}

int main(int argc, char **argv) {
	// Declaring Variables
	int i;

	// Create a thread for each philosopher
	pthread_t philosopher[PHILOSOPHERS];

	// Malloc for a semaphore struct
	Forks *fork_sem = malloc(sizeof(Forks));

	// Malloc for each semaphores
	fork_sem->fork_one = malloc(sizeof(sem_t));
	fork_sem->fork_two = malloc(sizeof(sem_t));
	fork_sem->fork_three = malloc(sizeof(sem_t));
	fork_sem->fork_four = malloc(sizeof(sem_t));
	fork_sem->fork_five = malloc(sizeof(sem_t));

	// Initialize the semaphores
	// 1 is the number of threads that
	// can access the fork_sem at a given time
	sem_init(fork_sem->fork_one, 0, 1);
	sem_init(fork_sem->fork_two, 0, 1);
	sem_init(fork_sem->fork_three, 0, 1);
	sem_init(fork_sem->fork_four, 0, 1);
	sem_init(fork_sem->fork_five, 0, 1);


	// Create threads
	for (i = 0; i < PHILOSOPHERS; i++) {
		arguments *args = malloc(sizeof(arguments));
		args->index = i;
		args->fork_sem = fork_sem;
		pthread_create(&(philosopher[i]), NULL, philosopher_process, (void *) args);
	}

	for (i = 0; i < PHILOSOPHERS; i++) {
		pthread_join(philosopher[i], NULL);
	}

	sem_destroy(fork_sem->fork_one);
	sem_destroy(fork_sem->fork_two);
	sem_destroy(fork_sem->fork_three);
	sem_destroy(fork_sem->fork_four);
	sem_destroy(fork_sem->fork_five);
	return 0;
}
