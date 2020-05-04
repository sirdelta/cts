/**
 * 
 * @file config.c
 * Command line arguments, programm settings and environment,
 * management of the configuration file.
 * 
 * This file manages the program parameters and set up the environment
 * before launching the simulation.
 * 
 * @see config.h
 * @version 1.2
 * 
 * ********************************************************* */
#include <stdio.h>
#include <stdlib.h>
#include "../inc/config.h"

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
int analyze_command_line_args (int argc, char * argv[]) {
	time_t timestamp  = time (NULL);
	int i = 0, cmd;
    char * near = "";

	time (&timestamp);

	nbMaxCars = DEFAULT_NB_CARS;
	timelapseNewCars = DEFAULT_MAX_TIMELAPSE;
	timeSwitchWay = DEFAULT_WAITING_TIME;

	/* No argument specified: set the interactive mode with default value. */
	if (argc < 2) {
		simuAutoMode = 0;
	}

	/* First check */
	for (i = 1; i < argc; i++) {
		cmd = (int) strtol (argv[i], &near, 10);
		if (strcmp (near, "") != 0) {	/* The argument is a string. */
			if (strlen (argv[i]) <= 3) {	/* Maybe an option... */
				if (strchr (argv[i], '-') == NULL) {
					fprintf (stderr, "Unknow option at index %d, use -h for help\n", i+1);
					return -1;
				}
			}
		} else {	/* The argument is a number. */
			if (strchr (argv[i-1], '-') == NULL) {	/* Check if there are an option before... */
				fprintf (stderr, "Invalid command at index %d, use -h for help\n", i+1);
				return -1;
			}
		}
	}       

	/* Second check and setting up */
    while ((cmd = getopt (argc, argv, "n:a:t:vhm")) != EOF) {
    	switch (cmd) {
			case 'n':	/* Number of cars option. */
				nbMaxCars = (int) strtol (optarg, &near, 10);
				if (nbMaxCars < 0 || nbMaxCars > 50) {
					fprintf (stderr, "Unauthorized number at index %d, use -h for help\n", optind);
					return -1;
				}
				break;
			case 'a':	/* Automatic mode option */
				simuAutoMode = 1;
				/* A timelapse could be specified... */
				timelapseNewCars = (int) strtol (optarg, &near, 10);
				if (optarg != near) {
					if (timelapseNewCars < 0 || timelapseNewCars > 10000000) {
						fprintf (stderr, "Unauthorized number at index %d, use -h for help\n", optind);
						return -1;
					}
				}
				break;
			case 't':	/* Timelapse before the traffic light go to green */
				timeSwitchWay = (int) strtol (optarg, &near, 10);
				if (timeSwitchWay < 0 || timeSwitchWay > 1000000) {
					fprintf (stderr, "Unauthorized number at index %d, use -h for help\n", optind);
					return -1;
				}
				break;
			case 'v':	/* Version */
				printf ("Crossroad Trafic Simulation v%f\n", VERSION);
				return 0;
			case 'h':	/*  Help menu */
				help_menu ();
				return 0;
			case 'm': 	/* User's manual */
				manual ();
				return 0;
			case '?':
			default :
                fprintf (stderr, "Expected argument after option at index %d, use -h for help\n", optind);
				return -1;
		}
	}

	/* Analyze done: start with configured settings. */
	puts (" ==== CTS ==================================");
	printf (" SESSION: %s", ctime (&timestamp));
	if (simuAutoMode)
		puts (" MODE: AUTOMATIC\n");
	else
		puts (" MODE: INTERACTIVE\n");
	puts (" CONFIGURATION");
	printf (" Number of cars: %d\n", nbMaxCars);
	printf (" Timelapse new arrival of a car: %d ms\n", timelapseNewCars);
	printf (" Timelapse traffic light: %d ms\n", timeSwitchWay);
	puts (" ===========================================\n");

	return 1;
}

/**
 * Display the program's help menu.
 */
void help_menu () {
	puts (" ==== CTS Help menu ============================");

	puts ("\n  -a [NUMBER]");
	puts ("\tStart the program in automatic mode. To change the timelpase of");
	puts("\tthe arrival of the cars, specify the new value in milliseconds.");
	printf ("\t(i) Default timelapseNewCars: %d ms\n", DEFAULT_MAX_TIMELAPSE);

	puts ("\n  -n [NUMBER]");
	puts ("\tSets the maximum number of cars which can wait at a traffic light.");
	puts ("\tBy default this value is limited to 50 cars.");
	puts ("\t/!\\ This option is required to start the simulation.");
	printf ("\t(i) Default number of cars: %d\n", DEFAULT_NB_CARS);

	puts ("\n  -t [NUMBER]");
	puts ("\tSpecify the minimum waiting time before going to the green light.");
	puts ("\tBy default this value is limited to 10 seconds. The duration must");
	puts ("\tbe set in milliseconds.");
	printf ("\t(i) Default duration: %d ms\n", DEFAULT_WAITING_TIME);

	puts ("\n  -h");
	puts ("\tDisplay the program's help menu.");

	puts ("\n  -m");
	puts ("\tDisplay the simulation's manual.");

	puts ("\n  -v");
	puts ("\tShow the current version of the program.");

	puts ("\n ===============================================");
}

/**
 * Display the program's user manual.
 */
void manual () {
	puts (" ==== CTS Manual ===============================");

	puts ("\n  * Interactive Mode (by default)");
	puts ("\tArrivals of cars on the lanes are manually choosed by typing a");
	puts ("\tspecific keyboard.");
	puts ("\t - Key [ 1 ]: the main lane");
	puts ("\t - Key [ 2 ]: the second lane");

	puts ("\n  * Automatic Mode");
	puts ("\tArrivals of cars on the lanes happen randomly from a maximum");
	puts ("\ttimelapse value in milliseconds. By default this value is limited");
	puts ("\tto 1 s. To activate the mode and change this timelapse, use option");
	puts ("\t\"-a\" in command line.");

	puts ("\n  (i) For more details about parameters and settings, see the help");
	puts ("      menu (option -h)");

	puts ("\n ===============================================");
}