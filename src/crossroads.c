/**
 * 
 * @file crossroads.c
 * Organization of the crossroad's lanes, change of junction and traffic lights.
 * 
 * This file is all about circulation on the crossroad's lane.
 * 
 * @see crossroads.h
 * @version 9.1
 * 
 * ********************************************************* */
#include <stdio.h>
#include <stdlib.h>
#include "../inc/crossroads.h"

/**
 * Manages the intersection.
 * 
 * Each lane of the crossroad is a processus. There are notify when the traffic
 * light change to red. At the end of the circulation, an end signal is send to
 * these processus.
 * 
 * @param timeSwitchWay the minimum waiting time before going to the green light
 */
void manage_junction (int timeSwitchWay) {
	time_t timestamp = time (NULL);
	struct tm * timeStruct;
	struct timeval start, end;

	int priority = 1;	/* Ensure shifting between the two lanes. */

	time (&timestamp);
	timeStruct = localtime (&timestamp);
	gettimeofday (&start, NULL);

	P (mutex[0]);
	shared->onRedLight = priority;
	V (mutex[0]);

	do {
		gettimeofday (&end, NULL);
		printf (
			" T%02d:%02d:%010ld|\tCARREFOUR : le feu %d passe au rouge\n",
			timeStruct->tm_min,
			timeStruct->tm_sec,
			(end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec),
			priority+1
		);

		priority = (priority == 1) ? 0 : 1;

		gettimeofday (&end, NULL);
		printf (
			" T%02d:%02d:%010ld|\tCARREFOUR : le feu %d passe au vert\n",
			timeStruct->tm_min,
			timeStruct->tm_sec,
			(end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec),
			priority+1
		);

		/* Going green: update the number of car waiting at the traffic lights. */
		P (mutex[1]);
		if (shared->nbWaitingCars != 0) {
			/* Signal child process to release all waiting cars. */
			kill (pids[2], RELEASE_CARS);

			gettimeofday (&end, NULL);
			printf (
				" T%02d:%02d:%010ld|\tCARREFOUR : On libère %d voiture(s)\n",
				timeStruct->tm_min,
				timeStruct->tm_sec,
				(end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec),
				shared->nbWaitingCars
			);

			shared->nbWaitingCars = 0;
		}
		V (mutex[1]);

		/* Give the priority to the lane in green light. */
		V (lane[priority]);
		usleep (timeSwitchWay);
		P (canAccess);
	} while (!shared->stopSig);

	/* If it's still waiting cars, signal child process to liberate them. */
	kill (pids[2], RELEASE_CARS);

	puts (" FIN CARREFOUR\n");
}

/**
 * Allow traffic on one lane.
 * Each lane of the crossroad use this function to to allow the passage of vehicles.
 * 
 * @param i the lane's number
 * @param timeSwitchWay the minimum waiting time before going to the green light
 */
void run_circulation (int i, int timeSwitchWay) {
	do {
		P (lane[i]);

		if (shared->stopSig)
			break;

		/* Cars come and go ... */
		usleep (timeSwitchWay);

		/* Going red: update of the "red light flag" to signal this. */
		P (mutex[0]);
		shared->onRedLight = i;
		V (mutex[0]);

		V (canAccess);
	} while (!shared->stopSig);

	/* The program is stopped: release semaphores. */
	V (mutex[0]);
	V (lane[i]);
	V (canAccess);
}

/**
 * Simply update the user's lane selection.
 * 
 * The information provided on the command line is analyzed at any time.
 * If the key entered is valid, the choice is saved and the car generator
 * is warned.
 */
void user_lane_choice () {
	unsigned char previous = '1', next = '\0';

	P (mutex[2]);
	shared->userCmdInterMode = '1';
	V (mutex[2]);

	do {
		next = getchar ();

		if (next != '\n') {
			/* Save the current value only if it's valid. */
			previous = next;
		} else {
			/* Analyze the previous value only if the '\n' is found */
			if (previous == LANE_ONE_KEY || previous == LANE_TWO_KEY) {
				P (mutex[2]);
				shared->userCmdInterMode = previous;
				V (mutex[2]);

				/* Signal brother process to use the new lane for new cars/threads. */
				kill (pids[2], SWITCH_LANE);
			}
		}
	} while (shared->stopSig || next != EOF);
}

/**
 * Toggle the program's end flag.
 * 
 * @param sigNum a signal associated to the end's flag
 */
void crossroads_toggle_stop (int sigNum) {
	if (sigNum == STOP_PROG)
		puts ("\n\t[ INTERRUPT SIGNAL ]\n");
	
	P (mutex[0]);
	P (mutex[1]);
	shared->stopSig = 1;
	V (mutex[0]);
	V (mutex[1]);
}