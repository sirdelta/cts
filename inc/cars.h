/**
 * 
 * @file cars.h
 * Driving cars simulation, management of car arrivals, passage
 * to traffic lights.
 * 
 * This file declares the functions used to simulate cars,
 * which are represented by threads.
 * 
 * @see param.h
 * @see ipcTools.h
 * @version 9.1
 * 
 * ********************************************************* */
#ifndef __cars_H
	#define __cars_H

	#include <pthread.h>
	#include <semaphore.h>

	/**
	 * Call global parameters.
	 */
	#include "../inc/param.h"

	/**
	 * Call the functions which are all about semaphores and shared memory.
	 */
	#include "../inc/ipcTools.h"

	/**
	 * Table of threads used to represent driving cars.
	 */
	pthread_t threads[50];	

	/**
	 * Mutex which protects a critical section.
	 */
	pthread_mutex_t goMut;

	/**
	 * Condition which test if a car can pass or not on a traffic light.
	 */
	pthread_cond_t goCond;

	/**
	 * Crossroad's lane choose by the user during interactive mode.
	 * All new cars must arrive in this lane.
	 */
	int laneUserChoice;

	/**
	 * Generate the cars/threads.
	 * 
	 * Allow waiting cars to go when the traffic light is green. Waits for
	 * the last one to finish before signaling the parent process.
	 * 
	 * @param nbCars the number of cars to be generated
	 * @param timelapseNewCars the maximum waiting time before a new car appear
	 * 
	 * @return	0 if success, a specific number if an error occured
	 */
	int generate_cars (int nbCars, int timelapseNewCars);

	/**
	 * Simulates the behavior of a car.
	 * 
	 * This function checks whether or not the conditions allow to pass or not,
	 * and acts accordingly. 
	 * 
	 * @param i The number of the car/thread
	 * 
	 * @return the id of the thread 
	 */
	void * driving_car (void * i);

	/**
	 * Unlock the cars/threads waiting at a traffic light.
	 * 
	 * @param sigNum the signal associated to the calling function
	 */
	void cars_toggle_go (int sigNum);

	/**
	 * Update the lane on which the new generated cars/threads must arrive.
	 * 
	 * @param sigNum the signal associated to the calling function
	 */
	void lane_toggle_switch (int sigNum);

#endif