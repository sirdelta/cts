/**
 * 
 * @file main.h
 * The main part of the program.
 * 
 * @see config.h
 * @see crossroads.h
 * @see cars.h
 * @version 9.4
 * 
 * ********************************************************* */
#ifndef __main_H
	#define __main_H

	/**
	 * Call the functions for configuring the environment
	 * before starting the program.
	 */
	#include "../inc/config.h"

	/**
	 * Call the functions used to manage the crossroad. 
	 */
	#include "../inc/crossroads.h"

	/**
	 * Call the functions used to manage cars arrivals. 
	 */
	#include "../inc/cars.h"

	/**
	 * Destroy all semaphores, mutex or shared variables created
	 * during program's execution.
	 * Cleanup... because we don't want to catch the virus  !
	 *  
	 * @param returnFalg the return value to end the program
	 * @param nbDel the number of variable to be destroy
	 * @param keyDel the key which refer to the last variable created with success
	 * 				 (especially used to destroy shared memory)
	 */
	void massive_cleanup (int returnFlag, int nbDel, key_t keyDel);

#endif