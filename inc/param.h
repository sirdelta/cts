/**
 * 
 * @file param.h
 * Settings and default programm's parameters
 * 
 * This file declares the default's parameters used in during
 * the simulation.
 * 
 * @version 5.2
 * 
 * ********************************************************* */
#ifndef __param_H
	#define __param_H

	#include <unistd.h>
	#include <time.h>
	#include <sys/time.h>
	#include <sys/types.h>
	#include <sys/wait.h>
	#include <signal.h>

	/**
	 * Version of the current program.
	 */
	#define VERSION 9.4

	/**
	 * Used as the default minimum waiting time before going to the green light.
	 */
	#define DEFAULT_WAITING_TIME 5000000 // 5s

	/**
	 * Uses as the default maximum waiting time before the arrival of a new car
	 * on a traffic light.
	 */
	#define DEFAULT_MAX_TIMELAPSE 2000000 // 3s

	/**
	 * Used as the default maximum number of cars waiting at a traffic light.
	 */
	#define DEFAULT_NB_CARS 30

	/**
	 * Define the mode of the simulation:
	 * 		0) interactive
	 * 		1) automatic
	 */
	int simuAutoMode;

	/**
	 * Table containing all chidren's process Id:
	 * 		0)	Crossroad way one PID
	 * 		1)	Crossroad way two PID
	 * 		2)	Cars's manager PID
	 * 		3)	User's command manager PID (interactive mode)
	 */
	pid_t pids[4];

	/**
	 * Shared variables used for communication betwwen all process.
	 * 
	 * @param onRedLight the red light flag
	 * @param nbWaitingCars the number of waiting cars at a red traffic light
	 * @param userCmdInterMode the current lane choose by the user durring the simulation
	 * @param stopSig the stop simulation flag
	 */
	typedef struct {
		int onRedLight;
		int nbWaitingCars;
		unsigned char userCmdInterMode;
		int stopSig;
	} Shared;

	/** 
	 * Pointer to the bunch of shared variables.
	 */
	Shared * shared;

	/**
	 * Table of mutex that protect shared variables:
	 * 		0)	The "red light flag" variable
	 * 		1)	The "number of waiting cars" variable
	 * 		2)	The "user's lane choice" variable	
	 */
	int mutex[3];

	/**
	 * The keyboard to select the lane 1.
	 */
	#define LANE_ONE_KEY '1'

	/**
	 * The keyboard to select the lane 2.
	 */
	#define LANE_TWO_KEY '2'

	/**
	 * Signal to update the defined lane where the new cars must arrive.
	 */
	#define SWITCH_LANE SIGUSR1	// Value: 10

	/**
	 * Signal to realease cars after traffic light going green.
	 */
	#define RELEASE_CARS SIGUSR2	// Value: 12

	/**
	 * Signal to end circulation.
	 */
	#define INTERRUPT_CIRCULATION SIGCHLD	// Value: 17

	/**
	 * Signal to end the program.
	 */
	#define STOP_PROG SIGINT	// Value: 2

#endif