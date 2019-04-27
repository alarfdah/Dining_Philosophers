/**
 * Date: 04/29/2019
 * Class: CS5541
 * Assignment: Assignment 5 - Dining_Philosophers
 * Author: Ahmed Radwan
 */
#ifndef SEMAPHORE_STRUCT_H
#define SEMAPHORE_STRUCT_H
#include <semaphore.h>
typedef struct semaphore_struct {
    sem_t *fork_one;
    sem_t *fork_two;
    sem_t *fork_three;
    sem_t *fork_four;
    sem_t *fork_five;
} Semaphore;
#endif