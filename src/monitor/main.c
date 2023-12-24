/*
 * The License to Kill Project
 *
 * Copyright (C) 2021 Alain Lebret <alain.lebret [at] ensicaen [dot] fr>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <fcntl.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#include "cell.h"
#include "character.h"
#include "memory.h"
#include "monitor.h"
#include "monitor_common.h"

#include "spy_simulation.h"
#include "simulation_signals.h"

#define MAP_WRITE 0x0002

extern WINDOW* main_window;
extern int old_cursor;

/**
 * \file main.c
 *
 * \brief Creates a new "Terminal User Interface" using ncurses and associated
 * with the given shared mem.
 *
 * The size of the terminal must have at least 40 rows and 145 columns. It
 * is decomposed in four sub-windows:
 * - The "city map and status" window (upper-left)
 * - The "character information" window (upper-right)
 * - The "mailbox content" window (bottom-left)
 * - The "enemy country monitor" window (bottom-right)
 * 'Q', 'q' and 'Esc' keys are used to exit from the TUI.
 */
int main(void) {
    int rows;
    int cols;
    int key;
    monitor_t* monitor;

    /* ---------------------------------------------------------------------- */
    /* The following code only allows to avoid segmentation fault !           */
    /* Change it to access to the real shared mem.                         */

    //mem = (memory_t*)malloc(sizeof(memory_t));

    memory_t *memory = open_shared_memory();
    set_memory(memory);


    memory_t* p = mmap(NULL, sizeof(memory_t), MAP_WRITE, MAP_SHARED, memory, 0);
    if (p == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    memory->memory_has_changed = 1;
    clear_city(&(memory->city_map));
    /*print_city(&(p->city_map));*/
    init_city(&(memory->city_map));
    /*print_city(&(p->city_map));*/

    memory->step = 100;

    /* ---------------------------------------------------------------------- */

    monitor = (monitor_t*) malloc(sizeof(monitor_t));
    monitor->has_to_update = 0;

    set_timer();
    set_signals();

    if ((main_window = initscr()) == NULL) {
        quit_after_error("Error initializing library ncurses!");
    }

    clear(); /* Start from an empty screen. */
    cbreak(); /* No line buffering, pass every key pressed. */
    noecho(); /* Do not echo the keyboard input. */
    old_cursor = curs_set(0); /* Use and invisible cursor. */
    keypad(stdscr, TRUE); /* Allows functions keys, arrows, etc. */

    start_color(); /* Allow using colors... */
    create_color_pairs(); /* ... and create color pairs to use */

    if (!is_terminal_size_larger_enough(&rows, &cols)) {
        quit_after_error("Minimal terminal dimensions: 45 rows and 140 columns!");
    }

    /* Initialize terminal user interface elements */
    init_monitor_elements(main_window, memory, rows, cols);

    /* Simulation configuration */
    int number_of_turns = 2016;
    // int turn_duration = calculate_turn_duration();

    /* Start my_timer */

    /*  Loop and get user input  */
    while (true) {
        key = getch();

        switch(key) {
        case 'Q':
        case 'q':
        case 27:
            memory->simulation_has_ended = 1;
            quit_nicely(NO_PARTICULAR_REASON);
        default: break;
        }
        if(memory->memory_has_changed) {
            update_values(memory);
            memory->memory_has_changed = 0;
        }
    }
}
