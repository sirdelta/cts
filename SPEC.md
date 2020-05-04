# Specifications

This document is a quick overview of the CTS project, covering the general structure of the project, the program execution and the project boundaries.

## Implementations

The objective of this project is to carry out a simulation of a crassroad's junction. Each file is specific to a very precise activity in the course of the simulation. Thus, the source folder contains three main files which, with their associated .h files, constitute the heart of the project:

* __crossroads.c__ allows the management of traffic on both lanes of the intersection.
* __cars.c__ contains the functions to generate cars, as well as to simulate the behaviour of motorists.
* __main.c__ contains the main part of the program to run the simulation.

Other files are added to these items. The __config.c file and its header__ are used to manage the information provided before launching the program. The __param.h file__ declares the global variables, constants and libraries used throughout the simulation. It also contains all the variables shared by all the processes.

For more details, consult documentation of these files.

## Running

Before starting the program, the user can provide the following information:

* The time to wait for the green light on a lane (`–t` option).
* The maximum time to wait for a new car to appear (`–a` option).
* The number of cars that must pass the intersection before the end of the program (`–n` option).

Regardless of the situation, these three elements are initialized to __their default values__, specified in the param.h file. The program can therefore be run without the user having to enter any input arguments.

In interactive mode, the user has the possibility to choose the lane on which the cars should arrive. To do this, he must __press the key specific to the lane of his choice, then press `enter`__. This is the mode provided by default. When a lane is chosen, the future generated vehicles will all position themselves on this lane. In this case, the user can change the lane on which the car should arrive at any time __until there are no more new cars__, indicating the end of the program.

In automatic mode, __the cars randomly choose the lane__ on which he wishes to arrive. In this case, the user is only a spectator of the comings and goings of the motorists. This mode is activated by selecting the `-a` option before the program is launched.

The simulation ends with a situation-specific return value.

__Return Value__ | __Condition__
---------------- | -------------
__1__ | The generation of an IPC key for semaphores and shared variables failed.
__2__ | the allocation of a IPC variable (semaphore or shared) did not work.
__3__ | Creating a process after calling the fork function did not work.
__4__ | allocating a mutex at thread level did not work.
__5__ | the condition attached to the mutex at thread level was not fulfilled.

If necessary, the user can interrupt the program at any time with `ctrl + c`.

## Limits

However, there are some limitations to the simulation process:
1. In some cases, the process managing the cars must prevent the main process  rom terminating itself when there are no more cars. This is not always the case.
1. In interactive mode, the user must delay the keyboard entries he makes, so as not to cause the simulation toend prematurely. The program is very sensitive to abrupt input. To avoid this problem, entries must be made within an unrestricted time frame. It is necessary to wait about 1 sec before the next input.
