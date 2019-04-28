/**
 * Date: 04/29/2019
 * Class: CS5541
 * Assignment: Assignment 5 - Dining_Philosophers
 * Author: Ahmed Radwan
 */
#ifndef SEATS_STRUCT_H
#define SEATS_STRUCT_H
#include <semaphore.h>

#define SEATS 5

typedef struct seats_struct {
  sem_t *seat_one;
  sem_t *seat_two;
  sem_t *seat_three;
  sem_t *seat_four;
} Seats;
#endif
