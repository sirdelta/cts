/**
 * 
 * @file main.c
 * The main part of the program.
 * 
 * @see main.h
 * @version 8.4
 * 
 * ********************************************************* */
#include <stdio.h>
#include <stdlib.h>
#include "../inc/main.h"

/**
 * The main part of the program:
 * 		- Set up environnement with command line arguments
 * 		- Create chlidren processes
 * 		- Wait the last child process
 * 		- Destroy all ressources
 */
int main (int argc, char * argv[]) {

	key_t key;	/* The first key */
	struct sigaction endCirc, endProg;	/* Used to signal the end of the program */
	int i, exec;

	/* COMMAND LINE ARGUMENTS */

	simuAutoMode = 0;	/* Choose the interactive mode by default */

	if ((i = analyze_command_line_args (argc, argv)) <= 0)
		return i;

	/* SET UP & ALLOCATIONS */

	if ((key = convertkey ("", -1)) == -1) {
	    perror ("Converkey failed");
		exit (1);
	}

	if ((lane[0] = semalloc (key+1, 0)) == -1) {
		perror ("Error creating semaphore");
		massive_cleanup (2, 0, 0);
	}
	if ((lane[1] = semalloc (key+2, 0)) == -1) {
		perror ("Error creating semaphore");
		massive_cleanup (2, 1, key);
	}
	if ((canAccess = semalloc (key, 0)) == -1) {
		perror ("Error creating semaphore");
		massive_cleanup (2, 2, key+1);
	}

	if ((mutex[0] = semalloc (key+3, 1)) == -1) {
		perror ("Error creating mutex");
		massive_cleanup (2, 3, key+2);
	}
	if ((mutex[1] = semalloc (key+4, 1)) == -1) {
		perror ("Error creating mutex");
		massive_cleanup (2, 4, key+3);
	}
	if ((mutex[2] = semalloc (key+5, 1)) == -1) {
		perror ("Error creating mutex");
		massive_cleanup (2, 5, key+4);
	}

	if (!(shared = (Shared *) shmalloc (key+6, sizeof (Shared)))) {
		perror ("Error creating shared memory");
		massive_cleanup (2, 6, key+5);
	}
	shared->nbWaitingCars = 0;
	shared->stopSig = 0;

	/* START SIMULATION */

	puts ("\t[ STRIKE <ENTER> TO START THE SIMULATION ]\n");
	getchar ();

	for (i = 0; i < 3; i++) {
		switch (pids[i] = fork ()) {
			case -1:
				perror ("fork failed");
				massive_cleanup (3, 99, key+6);
				exit (3);
			case 0:
				if (i != 2) {
					/* Process 0 and 1: the circulation on lane 1 and 2 */
					run_circulation (i, timeSwitchWay);
					exit (0);
				} else {
					/* Process 2: the arrivals of cars */
					exec = generate_cars (nbMaxCars, timelapseNewCars);
					if (exec)
						massive_cleanup (exec, 99, key+6);
					exit (exec);
				}
		}
	}

	/* Verify if it's an interactive mode. */
	if (!simuAutoMode) {
		switch (pids[3] = fork ()) {
			case -1:
				perror ("fork failed");
				massive_cleanup (3, 99, key+6);
				exit (3);
			case 0:
				/* Process 3: the user's command */
				user_lane_choice ();
				exit (0);
		}
	}

	/* Prepare to end the simulation ! */
	endCirc.sa_handler = crossroads_toggle_stop;
	sigaction (INTERRUPT_CIRCULATION, &endCirc, 0);
	endProg.sa_handler = crossroads_toggle_stop;
	sigaction (STOP_PROG, &endProg, 0);

	manage_junction (timeSwitchWay);	/* Coordinate child processes and switch the junction. */

	/* END PROGRAM */

	V (lane[0]);
	V (lane[1]);
	while (waitpid (0, 0, 0) < 0); /* Wait if a process doesn't finished yet. */

	massive_cleanup (0, 99, key+6);	/* Final cleanup of all ressources. */

	// return 0;
}

/**
 * Destroy all semaphores, mutex or shared variables created
 * during program's execution.
 * Cleanup... because we don't want to catch the virus  !
 *  
 * @param returnFalg the return value to end the program
 * @param nbDel the number of variable to be destroy
 * @param lastKey the key which refer to the last variable created with success
 * 				  (especially used to desallocate shared memory)
 */
void massive_cleanup (int returnFlag, int nbDel, key_t lastKey) {
	if (nbDel > 0) {
		switch (nbDel) {
			case 1:
				semfree (lane[0]);
				break;
			case 2:
				semfree (lane[0]);
				semfree (lane[1]);
				break;
			case 3:
				semfree (lane[0]);
				semfree (lane[1]);
				semfree (canAccess);
				break;
			case 4:
				semfree (lane[0]);
				semfree (lane[1]);
				semfree (canAccess);
				semfree (mutex[0]);
				break;
			case 5:
				semfree (lane[0]);
				semfree (lane[1]);
				semfree (canAccess);
				semfree (mutex[0]);
				semfree (mutex[1]);
				break;
			case 6:
				semfree (lane[0]);
				semfree (lane[1]);
				semfree (canAccess);
				semfree (mutex[0]);
				semfree (mutex[1]);
				semfree (mutex[2]);
				break;
			default:
				semfree (lane[0]);
				semfree (lane[1]);
				semfree (canAccess);
				semfree (mutex[0]);
				semfree (mutex[1]);
				semfree (mutex[2]);
				shmfree (lastKey);
		}
	}
	if (returnFlag)
		exit (returnFlag);
}