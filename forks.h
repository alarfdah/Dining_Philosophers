/**
 * Date: 04/29/2019
 * Class: CS5541
 * Assignment: Assignment 5 - Dining_Philosophers
 * Author: Ahmed Radwan
 */
#ifndef FORKS_STRUCT_H
#define FORKS_STRUCT_H
#include <semaphore.h>

#define FORKS 5

typedef struct forks_struct {
  sem_t *fork_one;
  sem_t *fork_two;
  sem_t *fork_three;
  sem_t *fork_four;
  sem_t *fork_five;
} Forks;
#endif
