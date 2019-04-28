/**
 * Date: 04/29/2019
 * Class: CS5541
 * Assignment: Assignment 5 - Dining_Philosophers
 * Author: Ahmed Radwan
 */
#ifndef ARGUMENTS_STRUCT_H
#define ARGUMENTS_STRUCT_H
#include <semaphore.h>
#include "forks.h"
#include "seats.h"
#define FORKS 5

typedef struct arguments_struct {
    int index;
    Forks *fork_sem;
    Seats *seat_sem;
} arguments;
#endif
