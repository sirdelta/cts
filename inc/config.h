/**
 * 
 * @file config.h
 * Command line arguments, programm settings and environment,
 * management of the configuration file.
 * 
 * This file declares the function to manages the program environnement.
 * The default settings are given in the "param.h" file. 
 * 
 * @see param.h
 * @version 1.2
 * 
 * ********************************************************* */
#ifndef __config_H
	#define __config_H

	#include <string.h>

	/**
	 * Call global parameters.
	 */
	#include "../inc/param.h"

	/**
	 * Extern variable used to retrieve the command line arguments.
	 */
	extern char * optarg;

	/**
	 * Extern variable used to swicth between two command line arguments.
	 */
	extern int optind;

	/**
	 * Environment variable which specified the minimum waiting time before
	 * going to the green light.
	 */
	int timeSwitchWay;

	/**
	 * Environment variable which specified the maximum waiting time before
	 * the arrival of a new car on a traffic light.
	 */
	int timelapseNewCars;

	/**
	 * Environment variable which specified the maximum number of cars
	 * waiting at a traffic light.
	 */
	int nbMaxCars;

	/**
	 * 
	 * Configure the program's environment from the information provided on
	 * the command line. Set up the program environment if this information is valid.
	 * 
	 * @param argc the number of arguments specified in the command line
	 * @param argv the array containing all arguments specified in the command line
	 * 
	 * @return 1 if the environnement is correctly set up, 0 if the manual is shown, -1 otherwise
	 */
	int analyze_command_line_args (int argc, char * argv[]);

	/**
	 * Display the program's help menu.
	 */
	void help_menu ();

	/**
	 * Display the program's user manual.
	 */
	void manual ();
	
#endif