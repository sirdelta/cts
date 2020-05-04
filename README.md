# Crossroad Traffic Simulation

CTSv9.4 is a small program simulating the circulation at a crossroad junction. The comings and goings of the cars are displayed and timed. The user has the possibility to interact during the simulation by choosing the lane on which the cars should arrive.
This program is an excellent way to learn how processes work under UNIX as well as the management of shared resources between processes.

## Build & Install

Generating the binary file is very simple. In the terminal, you just need to enter:
```bash
make
```

To install the executable in the current folder and in the user’s HOME directory, you must enter:
```bash
make install
```
This command requires specific permissions. As default, these permissions are read, write and execute for the owner, only read and execute for the group and for others.

To remove the executable in these both folders, you need to enter:
```bash
make uninstall
```

## Usage

The program offers two modes of simulation.



### Manual

### Command line options