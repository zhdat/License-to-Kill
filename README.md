
# "License to Kill" Project

## Description

This project is a complex simulation of a spy network operating in a fictional city. It implements a multiprocess 
application using various C programming techniques to simulate dynamic interactions among espionage agents, 
counter-espionage officers, and ordinary citizens. The simulation integrates mechanisms such as inter-process 
communication, thread synchronization, shared memory, and signal management.

![The Monitor Program](./doc/figures/spies_monitor.png)

## Features

* Simulation of different characters (spies, counter-espionage officers, citizens) moving around a city.
* Management of interactions between characters, including tracking, spying, and evasion attempts.
* Use of shared memory for tracking the state of the simulation.
* Inter-process communication using message queues and semaphores.

## Prerequisites

* GCC (GNU Compiler Collection) for compiling C code.
* UNIX-like operating system (Linux, macOS).
* Ncurses library for the monitor's user interface.

## Installation

For a quick installation and execution, use the following command:

```bash
make run
```

### Compiling the Project

* Clone the Git repository or download the project sources.
* Access the project's root directory.
* To compile the project, use the following command:
  ```bash
  make all
  ```

### Execution

After compiling, several executables will be generated. Here's how to run them:

Launch the main simulation process:

```bash
./main_program
```

### Cleaning

To clean up the compiled files, use the command `make distclean`:

```bash
make distclean
```

## Documentation

The project's documentation is available in the [doc](./doc/) directory.

You can also generate documentation from the project's sources using the following command:

```bash
make doc
```

## Authors

* Clément Jantet <clement.jantet@ecole.ensicaen.fr>
* Calliste Ravix <calliste.ravix@ecole.ensicaen.fr>
* Guillain Le Goff <guillain.le-goff@ecole.ensicaen.fr>
* Maxime Michel <maxime.michel@ecole.ensicaen.fr>
