CC=gcc
CFLAGS=-Wall -Wextra -pedantic -O2 -g

# Compilation under MacOS X or Linux
UNAME=$(shell uname -s)
ifeq ($(UNAME),Darwin)
    CPPFLAGS=-D_REENTRANT -I./include -I/opt/homebrew/opt/ncurses/include
    LDFLAGS=-L/opt/homebrew/opt/ncurses/lib -lncurses -lpthread -lm
endif
ifeq ($(UNAME),Linux)
    CPPFLAGS=-D_REENTRANT -I./include
    LDFLAGS=-lncurses -lpthread -lrt -lm
endif

.PHONY: all clean distclean run

all: bin/main_program bin/monitor bin/spy_simulation bin/timer bin/enemy_country bin/testing bin/enemy_spy_network bin/citizen_manager bin/counterintelligence_officer

# ----------------------------------------------------------------------------
# MAIN PROGRAM
# ----------------------------------------------------------------------------
bin/main_program: obj/main.o
	rm -rf /dev/shm/nothinghere /dev/shm/sem.*
	$(CC) $^ -o $@ $(LDFLAGS)

obj/main.o: src/main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# MONITOR
# ----------------------------------------------------------------------------
bin/monitor: obj/monitor_main.o obj/monitor.o obj/monitor_common.o obj/common/logger.o obj/character/character.o obj/cell.o obj/common/memory.o obj/simulation_signals.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/monitor_main.o: src/monitor/main.c include/monitor.h include/monitor_common.h include/memory.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/monitor.o: src/monitor/monitor.c include/monitor.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/monitor_common.o: src/monitor/monitor_common.c include/monitor_common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# COMMON OBJECTS FILES
# ----------------------------------------------------------------------------

obj/common/logger.o: src/common/logger.c include/logger.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/common/memory.o: src/common/memory.c include/memory.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/common/tools.o: src/common/tools.c include/tools.h include/memory.h include/logger.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c -lm

# ----------------------------------------------------------------------------
# SPY SIMULATION
# ----------------------------------------------------------------------------
bin/spy_simulation: obj/character_factory_main.o obj/character_factory.o obj/character/character.o obj/cell.o obj/common/logger.o obj/spy_simulation.o obj/common/memory.o obj/simulation_signals.o
	$(CC) $(CPPFLAGS2) $(CFLAGS) $^ -o $@ -lm

obj/character_factory_main.o: src/spy_simulation/main.c include/character_factory.h include/logger.h include/memory.h include/spy_simulation.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/character_factory.o: src/spy_simulation/character_factory.c include/character_factory.h include/logger.h include/memory.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/spy_simulation.o: src/spy_simulation/spy_simulation.c include/spy_simulation.h include/logger.h include/memory.h include/character_factory.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/simulation_signals.o: src/spy_simulation/simulation_signals.c include/simulation_signals.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# CHARACTER OBJECTS FILES
# ----------------------------------------------------------------------------

obj/character/character.o: src/character/character.c include/character.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/cell.o: src/spy_simulation/cell.c include/cell.h include/character.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# ENEMY SPY NETWORK
# ----------------------------------------------------------------------------

obj/character/enemy_spy_network.o: src/character/enemy_spy_network.c include/enemy_spy_network.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/enemy_spy_network/main.o: src/enemy_spy_network/main.c include/enemy_spy_network.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

bin/enemy_spy_network: obj/enemy_spy_network/main.o obj/character/enemy_spy_network.o obj/common/memory.o obj/common/tools.o obj/common/logger.o obj/character/character.o obj/cell.o obj/simulation_signals.o
	$(CC) $^ -o $@ $(LDFLAGS)

# ----------------------------------------------------------------------------
# CITIZEN MANAGER
# ----------------------------------------------------------------------------

obj/character/citizen_manager.o: src/character/citizen_manager.c include/citizen_manager.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/citizen_manager/main.o: src/citizen_manager/main.c include/citizen_manager.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

bin/citizen_manager: obj/citizen_manager/main.o obj/character/citizen_manager.o obj/common/memory.o obj/common/tools.o obj/common/logger.o obj/character/character.o obj/cell.o obj/simulation_signals.o
	$(CC) $^ -o $@ $(LDFLAGS)

# ----------------------------------------------------------------------------
# COUNTERINTELLIGENCE OFFICER
# ----------------------------------------------------------------------------

obj/character/counterintelligence_officer.o: src/character/counterintelligence_officer.c include/counterintelligence_officer.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/counterintelligence_officer/main.o: src/counterintelligence_officer/main.c include/counterintelligence_officer.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

bin/counterintelligence_officer: obj/counterintelligence_officer/main.o obj/character/counterintelligence_officer.o obj/common/memory.o obj/common/tools.o obj/common/logger.o obj/character/character.o obj/cell.o obj/simulation_signals.o
	$(CC) $^ -o $@ $(LDFLAGS)

# ----------------------------------------------------------------------------
# TIMER OBJECTS FILES
# ----------------------------------------------------------------------------

bin/timer: obj/timer/timer.o obj/timer/main.o obj/common/memory.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/timer/timer.o: src/timer/timer.c include/timer.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/timer/main.o: src/timer/main.c include/timer.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c


# ----------------------------------------------------------------------------
# ENEMY COUNTRY
# ----------------------------------------------------------------------------

bin/enemy_country: obj/enemy_country/enemy_country.o obj/common/memory.o obj/enemy_country/main.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/enemy_country/enemy_country.o: src/enemy_country/enemy_country.c include/enemy_country.h include/memory.h include/common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/enemy_country/main.o: src/enemy_country/main.c include/enemy_country.h include/common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c


bin/testing: obj/testing.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/testing.o: src/enemy_country/testing.c include/enemy_country.h include/common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# CLEANING
# ----------------------------------------------------------------------------
run: distclean all
	./bin/main_program

clean:
	rm -rf obj/*.o
	rm -rf obj/common/*.o
	rm -rf obj/character/*.o
	rm -rf obj/timer/*.o
	rm -rf obj/enemy_country/*.o

distclean: clean
	rm -rf bin/*
	rm -rf /dev/shm/nothinghere /dev/shm/sem.*
