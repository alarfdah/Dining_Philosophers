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
#define SERVINGS 10


int get_seat(sem_t *seat_one, sem_t *seat_two, sem_t *seat_three, sem_t *seat_four, int index) {
	struct timespec time;
	int timedwaitResult = 0;


	clock_gettime(CLOCK_REALTIME, &time);
	time.tv_sec += 0;
	timedwaitResult = sem_timedwait(seat_one, &time);
	if (timedwaitResult != -1) {
			sleep(1);
			printf("Philosopher %d grabbed seat ONE.\n", index);
			return 1;
	}

	clock_gettime(CLOCK_REALTIME, &time);
	time.tv_sec += 0;
	timedwaitResult = sem_timedwait(seat_two, &time);
	if (timedwaitResult != -1) {
			sleep(1);
			printf("Philosopher %d grabbed seat ONE.\n", index);
			return 1;
	}

	clock_gettime(CLOCK_REALTIME, &time);
	time.tv_sec += 0;
	timedwaitResult = sem_timedwait(seat_three, &time);
	if (timedwaitResult != -1) {
			sleep(1);
			printf("Philosopher %d grabbed seat ONE.\n", index);
			return 1;
	}

	clock_gettime(CLOCK_REALTIME, &time);
	time.tv_sec += 0;
	timedwaitResult = sem_timedwait(seat_four, &time);
	if (timedwaitResult != -1) {
			sleep(1);
			printf("Philosopher %d grabbed seat ONE.\n", index);
			return 1;
	}

	sleep(1);
	printf("Philosopher %d couldn't grab a seat.\n", index);
	return 0;
}

int get_forks(sem_t *fork_left, sem_t *fork_right, int index) {
	int ate = 0;

	// Grab fork to the left
	sem_wait(fork_left);
	// printf("Philosopher %d grabbed the LEFT fork\n", index);

	// Grab the fork to the right
	sem_wait(fork_right);
	// printf("Philosopher %d grabbed the RIGHT fork\n", index);

	// Eat
	ate = 1;
	return ate;
}

void *philosopher_process(void * myargs) {
	arguments *args = (arguments *)myargs;
	int ate = 0;
	int i = 0;
	int index = args->index;

	sem_t *fork_left;
	sem_t *fork_right;

	sem_t *seat_one = args->seat_sem->seat_one;
	sem_t *seat_two = args->seat_sem->seat_two;
	sem_t *seat_three = args->seat_sem->seat_three;
	sem_t *seat_four = args->seat_sem->seat_four;

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

	printf("PHILOSOPHER %d\n", index);

	int is_seated = get_seat(seat_one, seat_two, seat_three, seat_four, index);
	do {
		if (is_seated) {
			ate += get_forks(fork_left, fork_right, index);
			sem_post(fork_left);
			sem_post(fork_right);
			sem_close(fork_left);
			sem_close(fork_right);
		}
		i++;
	} while (i < SERVINGS);
	printf("Philosohpher %d ate %d times.\n", index, ate);
	sem_post(seat_one);
	sem_post(seat_two);
	sem_post(seat_three);
	sem_post(seat_four);
	sem_close(seat_one);
	sem_close(seat_two);
	sem_close(seat_three);
	sem_close(seat_four);

	pthread_exit(NULL);
}

int main(int argc, char **argv) {
	// Declaring Variables
	int i;

	// Create a thread for each philosopher
	pthread_t philosopher[PHILOSOPHERS];

	// Malloc for a Forks semaphore struct
	Forks *fork_sem = malloc(sizeof(Forks));

	// Malloc for a Seats semaphore struct
	Seats *seat_sem = malloc(sizeof(Seats));

	// Malloc for each fork semaphores
	fork_sem->fork_one = malloc(sizeof(sem_t));
	fork_sem->fork_two = malloc(sizeof(sem_t));
	fork_sem->fork_three = malloc(sizeof(sem_t));
	fork_sem->fork_four = malloc(sizeof(sem_t));
	fork_sem->fork_five = malloc(sizeof(sem_t));

	// Malloc for each seat semaphore
	seat_sem->seat_one = malloc(sizeof(sem_t));
	seat_sem->seat_two = malloc(sizeof(sem_t));
	seat_sem->seat_three = malloc(sizeof(sem_t));
	seat_sem->seat_four = malloc(sizeof(sem_t));

	// Initialize the semaphores
	// 1 is the number of threads that
	// can access the fork_sem at a given time
	sem_init(fork_sem->fork_one, 0, 1);
	sem_init(fork_sem->fork_two, 0, 1);
	sem_init(fork_sem->fork_three, 0, 1);
	sem_init(fork_sem->fork_four, 0, 1);
	sem_init(fork_sem->fork_five, 0, 1);


	sem_init(seat_sem->seat_one, 0, 1);
	sem_init(seat_sem->seat_two, 0, 1);
	sem_init(seat_sem->seat_three, 0, 1);
	sem_init(seat_sem->seat_four, 0, 1);


	// Create threads
	for (i = 0; i < PHILOSOPHERS; i++) {
		arguments *args = malloc(sizeof(arguments));
		args->index = i;
		args->fork_sem = fork_sem;
		args->seat_sem = seat_sem;
		// args->queue = &queue;
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
	
	sem_destroy(seat_sem->seat_one);
	sem_destroy(seat_sem->seat_two);
	sem_destroy(seat_sem->seat_three);
	sem_destroy(seat_sem->seat_four);
	return 0;
}
