# Crossroad Traffic Simulation

CTS is a small program simulating the circulation at a crossroad junction. The comings and goings of the cars are displayed and timed.
The user has the possibility to interact during the simulation by choosing the lane on which the cars should arrive.

This program is an excellent way to learn how processes under UNIX work, as well as the management of shared resources between processes.

## Build & Install

Generating the binary file is very simple. In the terminal, you just need to enter:
```bash
make
```

To install the executable in the current folder and in the user’s HOME directory, enter the command below:
```bash
make install
```
This command requires specific permissions. As default, these permissions are set as __read, write and execute for the owner, only read and execute for the group and for others__.

To remove the executable in these both folders, enter:
```bash
make uninstall
```

## Usage

The program runs into two modes of simulation, in interactive or in automatic. To configue the program before running, some options can be set.

### Interactive Mode (by default)

The arrivals of cars on the lanes are manually chose by striking a specific keyboard:
* `Key 1` for the main lane
* `Key 2` for the second lane

### Automatic Mode

The arrivals of cars on the lanes happen randomly from a maximum time-lapse value in milliseconds. By default, this value is limited to 1 s.

To activate the mode and change this time-lapse, use the option `-a` in command line.

For more details about parameters and settings to set before start a simulation, see the help menu option in command line (`–h` option).

### Command line options

The list of options is as follows:

```bash
-a [ NUMBER ]
```
Start the program in automatic mode. To change the time-lapse of the arrival of the cars, specify the new value in milliseconds.

```bash
-n [ NUMBER ]
```
Sets the maximum number of cars which can wait at a traffic light. By default, this value is limited to 50 cars. This option is required to start the simulation.

```bash
-t [ NUMBER ]
```
Specify the minimum waiting time before going to the green light. By default, this value is limited to 10 seconds. The duration must be set in milliseconds.

```bash
-h
```
Display the program's help menu.

```bash
-m
```
Display the simulation's manual.

```bash
-v
```
Show the current version of the program.

For more details about default input value, see the help menu option in command line (`–h` option).
