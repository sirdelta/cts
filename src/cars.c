/**
 * 
 * @file cars.c
 * Driving cars simulation, management of car arrivals, passage
 * to traffic lights.
 * 
 * This file is all about cars on the crossroads's lane.
 * 
 * @see cars.h
 * @version 9.1
 * 
 * ********************************************************* */
#include <stdio.h>
#include <stdlib.h>
#include "../inc/cars.h"

/**
 * Generate the cars/threads.
 * 
 * Allow waiting cars to go when the traffic light is green. Waits for
 * the last one to finish before signaling the parent process.
 * 
 * @param nbCars the number of cars to be generated
 * @param timelapseNewCars the maximum waiting time before a new car appear
 * @param mode
 * 
 * @return	0 if success, a specific number if an error occured
 */
int generate_cars (int nbCars, int timelapseNewCars) {
	time_t timestamp = time (NULL);
	struct tm * timeStruct;
	struct timeval start, end;

	struct sigaction go;	/*	Used when traffic light switch to green
								for allow cars to pass. */
	struct sigaction newLane;	/*	Used to update the defined lane for the
									cars/threads. */
	long car = 0;

	time (&timestamp);
	timeStruct = localtime (&timestamp);
	gettimeofday (&start, NULL);

	go.sa_handler = cars_toggle_go;
	sigaction (RELEASE_CARS, &go, 0);
	newLane.sa_handler = lane_toggle_switch;
	sigaction (SWITCH_LANE, &newLane, 0);
	
	/* Creation of the mutex and an attached condition. */
	if ((pthread_mutex_init (&goMut, 0)) == -1) {
		perror ("Error creating mutex");
		return 4;
	}
	if (pthread_cond_init (&goCond, 0) == -1) {
		perror ("Error creating threads condition");
		pthread_mutex_destroy (&goMut);
		return 5;
	}

	srandom (pthread_self ());	/* Initialize random generator */

	/* Creation of cars/threads */
	for (car = 0; car < nbCars; car++) {
		usleep (random() % timelapseNewCars);
		pthread_create (&threads[car], 0, driving_car, (void *) car);
	}
	/* No more car: destroy all threads and ressouces, and signal parent. */

	gettimeofday (&end, NULL);
	printf (
		" T%02d:%02d:%010ld|\t\tVOITURE : aucune nouvelle voiture en vue ...\n",
		timeStruct->tm_min,
		timeStruct->tm_sec,
		(end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec)
	);

	usleep (timelapseNewCars*2);	/* Enough to wait parent last instruction. */

	/* Want all thread child... */
	for (car = 0; car < nbCars; car++) {
		pthread_join (threads[car], 0);
	}
	pthread_mutex_destroy (&goMut);
	pthread_cond_destroy (&goCond);

	// while (!shared->stopSig)
		kill (getppid (), INTERRUPT_CIRCULATION);

	return 0;	
}

/**
 * Simulates the behavior of a car.
 * 
 * The car/thread positions itself in a lane, checks whether or not the conditions
 * allow to pass a traffic light or not, and acts accordingly.
 * 
 * @param i The number of the car/thread
 */
void * driving_car (void * i) {
	time_t timestamp = time (NULL);
	struct tm * timeStruct;
	struct timeval start, end;

	int laneChoice;

	time (&timestamp);
	timeStruct = localtime (&timestamp);
	gettimeofday (&start, NULL);

	if (!simuAutoMode) {
		pthread_mutex_lock (&goMut);

		/* Interactive simulation: the car/thread musts be in the defined lane */
		laneChoice = laneUserChoice;

		pthread_mutex_unlock (&goMut);
	} else {
		srandom (pthread_self ());	/* Initialize random generator */
		laneChoice = random() % 2;	
	}
	
	gettimeofday (&end, NULL);
	printf (
		" T%02d:%02d:%010ld|\t\tVOITURE : arrivée de la voiture %ld sur la voie %d\n",
		timeStruct->tm_min,
		timeStruct->tm_sec,
		(end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec),
		(long) i+1,
		laneChoice+1
	);

	if (shared->onRedLight == laneChoice) {
		gettimeofday (&end, NULL);
		printf (
			" T%02d:%02d:%010ld|\t\tVOITURE : la voiture %ld est en attente\n",
			timeStruct->tm_min,
			timeStruct->tm_sec,
			(end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec),
			(long) i+1
		);

		P (mutex[1]);
		shared->nbWaitingCars++;
		gettimeofday (&end, NULL);
		printf (
			" T%02d:%02d:%010ld|\t\tVOITURE : il y a %d voiture(s) en attente\n",
			timeStruct->tm_min,
			timeStruct->tm_sec,
			(end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec),
			shared->nbWaitingCars
		);
		V (mutex[1]);
	}

	/* If the traffic light is red, the car waits until it go to green. */
	while (shared->onRedLight == laneChoice) {
		pthread_mutex_lock (&goMut);

		/* Wait the parent-thread signal to pass the condition. */
		pthread_cond_wait (&goCond, &goMut);

		pthread_mutex_unlock (&goMut);
	}

	usleep (random() % 1000000);	/* It takes a while for the car to pass... */
	gettimeofday (&end, NULL);
	printf (
		" T%02d:%02d:%010ld|\t\tVOITURE : la voiture %ld est passée\n",
		timeStruct->tm_min,
		timeStruct->tm_sec,
		(end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec),
		(long) i+1
	);

	return 0;
}

/**
 * Unlock the cars/threads waiting at a traffic light.
 * 
 * @param sigNum the signal associated to the calling function
 */
void cars_toggle_go (int sigNum) {
	pthread_mutex_lock (&goMut);

	/* The parent/thread signal all waiting cars/threads to pass. */
	pthread_cond_broadcast (&goCond);

	pthread_mutex_unlock (&goMut);
}

/**
 * Update the lane on which the new generated cars/threads must arrive.
 * 
 * @param sigNum the signal associated to the calling function
 */
void lane_toggle_switch (int sigNum) {
	pthread_mutex_lock (&goMut);

	P (mutex[2]);
	if (shared->userCmdInterMode == LANE_ONE_KEY) {
		laneUserChoice = 0;
	} else if (shared->userCmdInterMode == LANE_TWO_KEY) {
		laneUserChoice = 1;
	}
	V (mutex[2]);
	printf ("\n\t[ COMMAND: CHOOSE LANE %d ]\n\n", laneUserChoice+1);

	pthread_mutex_unlock (&goMut);
}