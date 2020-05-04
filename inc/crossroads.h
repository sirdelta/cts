/**
 * 
 * @file crossroads.h
 * Organization of the crossroads lanes, change of junction and traffic lights.
 * 
 * This file declares the functions for the management of the crossroad's lanes.
 * The change of traffic light on each of the two lanes is done using semaphores.
 * 
 * @see param.h
 * @see ipcTools.h
 * @version 9.1
 * 
 * ********************************************************* */
#ifndef __crossroads_H
	#define __crossroads_H

	/**
	 * Call global parameters.
	 */
	#include "../inc/param.h"

	/**
	 * Call the functions which are all about semaphores and shared memory.
	 */
	#include "../inc/ipcTools.h"

	/**
	 * Table of semaphores associated to each lane of the crossroad.
	 */
	int lane[2];

	/**
	 * Semaphore associated to the red traffic light switching.
	 */
	int canAccess;

	/**
	 * Manages the intersection.
	 * 
	 * Each lane of the crossroad is a processus. There are notify when the traffic
	 * light change to red. At the end of the circulation, an end signal is send to
	 * these processus.
	 * 
	 * @param timeSwitchWay the minimum waiting time before going to the green light
	 */
	void manage_junction (int timeSwitchWay);

	/**
	 * Allow traffic on one lane.
	 * Each lane of the crossroad use this function to to allow the passage of vehicles.
	 * 
	 * @param i the lane's number
	 * @param timeSwitchWay the minimum waiting time before going to the green light
	 */
	void run_circulation (int i, int timeSwitchWay);
	
	/**
	 * Simply update the user's lane selection.
	 * 
	 * The information provided on the command line is analyzed at any time.
	 * If the key entered is valid, the choice is saved and the car generator
	 * is warned.
	 */
	void user_lane_choice ();
	
	/**
	 * Toggle the program's end flag.
	 * 
	 * @param sigNum a signal associated to the end's flag
	 */
	void crossroads_toggle_stop (int sigNum);

#endif