CC=gcc
CFLAGS=-w -Wall -Wextra -pedantic -O2 -g

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

.PHONY: all clean distclean run doc

all: bin/main_program bin/monitor bin/spy_simulation bin/timer bin/enemy_country bin/enemy_spy_network bin/citizen_manager bin/counterintelligence_officer tests/citizen_manager/test_citizen_manager tests/common/test_tools

# ----------------------------------------------------------------------------
# MAIN PROGRAM
# ----------------------------------------------------------------------------
bin/main_program: obj/main.o obj/common/logger.o
	rm -rf /dev/shm/nothinghere /dev/shm/sem.*
	$(CC) $^ -o $@ $(LDFLAGS)

obj/main.o: src/main.c include/logger.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# MONITOR
# ----------------------------------------------------------------------------
bin/monitor: obj/monitor/monitor_main.o obj/monitor/monitor.o obj/monitor/monitor_common.o obj/common/logger.o obj/spy_simulation/cell.o obj/common/memory.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/monitor/monitor_main.o: src/monitor/main.c include/monitor.h include/monitor_common.h include/memory.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/monitor/monitor.o: src/monitor/monitor.c include/monitor.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/monitor/monitor_common.o: src/monitor/monitor_common.c include/monitor_common.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# COMMON OBJECTS FILES
# ----------------------------------------------------------------------------

obj/common/logger.o: src/common/logger.c include/logger.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/common/memory.o: src/common/memory.c include/memory.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/common/tools.o: src/common/tools.c include/tools.h include/memory.h include/logger.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c -lm

# ----------------------------------------------------------------------------
# SPY SIMULATION
# ----------------------------------------------------------------------------
bin/spy_simulation: obj/spy_simulation/character_factory_main.o obj/spy_simulation/character_factory.o obj/spy_simulation/cell.o obj/common/logger.o obj/spy_simulation/spy_simulation.o obj/common/memory.o obj/common/tools.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/spy_simulation/character_factory_main.o: src/spy_simulation/main.c include/character_factory.h include/logger.h include/memory.h include/spy_simulation.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/spy_simulation/character_factory.o: src/spy_simulation/character_factory.c include/character_factory.h include/logger.h include/memory.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/spy_simulation/spy_simulation.o: src/spy_simulation/spy_simulation.c include/spy_simulation.h include/logger.h include/memory.h include/character_factory.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/spy_simulation/cell.o: src/spy_simulation/cell.c include/cell.h include/character.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# ENEMY SPY NETWORK
# ----------------------------------------------------------------------------

obj/enemy_spy_network/enemy_spy_network.o: src/enemy_spy_network/enemy_spy_network.c include/enemy_spy_network.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/enemy_spy_network/main.o: src/enemy_spy_network/main.c include/enemy_spy_network.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

bin/enemy_spy_network: obj/enemy_spy_network/main.o obj/enemy_spy_network/enemy_spy_network.o obj/common/memory.o obj/common/tools.o obj/common/logger.o obj/spy_simulation/cell.o
	$(CC) $^ -o $@ $(LDFLAGS)

# ----------------------------------------------------------------------------
# CITIZEN MANAGER
# ----------------------------------------------------------------------------

obj/citizen_manager/citizen_manager.o: src/citizen_manager/citizen_manager.c include/citizen_manager.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/citizen_manager/main.o: src/citizen_manager/main.c include/citizen_manager.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

bin/citizen_manager: obj/citizen_manager/main.o obj/citizen_manager/citizen_manager.o obj/common/memory.o obj/common/tools.o obj/common/logger.o obj/spy_simulation/cell.o
	$(CC) $^ -o $@ $(LDFLAGS)

# ----------------------------------------------------------------------------
# COUNTERINTELLIGENCE OFFICER
# ----------------------------------------------------------------------------

obj/counterintelligence_officer/counterintelligence_officer.o: src/counterintelligence_officer/counterintelligence_officer.c include/counterintelligence_officer.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/counterintelligence_officer/main.o: src/counterintelligence_officer/main.c include/counterintelligence_officer.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

bin/counterintelligence_officer: obj/counterintelligence_officer/main.o obj/counterintelligence_officer/counterintelligence_officer.o obj/common/memory.o obj/common/tools.o obj/common/logger.o obj/spy_simulation/cell.o
	$(CC) $^ -o $@ $(LDFLAGS)

# ----------------------------------------------------------------------------
# TIMER OBJECTS FILES
# ----------------------------------------------------------------------------

bin/timer: obj/timer/timer.o obj/timer/main.o obj/common/memory.o obj/common/logger.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/timer/timer.o: src/timer/timer.c include/timer.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/timer/main.o: src/timer/main.c include/timer.h include/logger.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c


# ----------------------------------------------------------------------------
# ENEMY COUNTRY
# ----------------------------------------------------------------------------

bin/enemy_country: obj/enemy_country/enemy_country.o obj/common/memory.o obj/enemy_country/main.o obj/common/logger.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/enemy_country/enemy_country.o: src/enemy_country/enemy_country.c include/enemy_country.h include/memory.h include/common.h include/logger.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/enemy_country/main.o: src/enemy_country/main.c include/enemy_country.h include/common.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# TESTS
# ----------------------------------------------------------------------------
tests/citizen_manager/test_citizen_manager: tests/citizen_manager/test_citizen_manager.o obj/citizen_manager/citizen_manager.o obj/common/memory.o obj/common/tools.o obj/common/logger.o obj/spy_simulation/cell.o obj/spy_simulation/character_factory.o obj/spy_simulation/spy_simulation.o
	$(CC) $^ -o $@ $(LDFLAGS)

tests/citizen_manager/test_citizen_manager.o: tests/citizen_manager/test_citizen_manager.c include/citizen_manager.h include/debug.h include/memory.h include/spy_simulation.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

tests/common/test_tools: tests/common/test_tools.o obj/common/tools.o obj/common/memory.o obj/common/logger.o obj/spy_simulation/cell.o obj/spy_simulation/character_factory.o obj/spy_simulation/spy_simulation.o
	$(CC) $^ -o $@ $(LDFLAGS)

tests/common/test_tools.o: tests/common/test_tools.c include/tools.h include/memory.h include/logger.h include/debug.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# DOCUMENTATION
# ----------------------------------------------------------------------------

doc: doc/doxyfile
	doxygen doc/doxyfile

# ----------------------------------------------------------------------------
# CLEANING
# ----------------------------------------------------------------------------
run: distclean all
	./bin/main_program

clean:
	rm -rf obj/*.o
	rm -rf obj/common/*.o
	rm -rf obj/timer/*.o
	rm -rf obj/enemy_country/*.o
	rm -rf obj/monitor/*.o
	rm -rf obj/spy_simulation/*.o

distclean: clean
	rm -rf bin/*
	rm -rf /dev/shm/nothinghere
	rm -rf /dev/shm/sem.*
	rm -rf doc/html
