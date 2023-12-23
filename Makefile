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

all: bin/main_program bin/monitor bin/character_factory

# ----------------------------------------------------------------------------
# MAIN PROGRAM
# ----------------------------------------------------------------------------
bin/main_program: obj/main.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/main.o: src/main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# MONITOR
# ----------------------------------------------------------------------------
bin/monitor: obj/monitor_main.o obj/monitor.o obj/monitor_common.o obj/logger.o obj/character.o obj/cell.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/monitor_main.o: src/monitor/main.c include/monitor.h include/monitor_common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/monitor.o: src/monitor/monitor.c include/monitor.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/monitor_common.o: src/monitor/monitor_common.c include/monitor_common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c



# ----------------------------------------------------------------------------
# COMMON OBJECTS FILES
# ----------------------------------------------------------------------------

obj/logger.o: src/common/logger.c include/logger.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c


# ----------------------------------------------------------------------------
# CHARACTER OBJECTS FILES
# ----------------------------------------------------------------------------
bin/character_factory: obj/character_factory_main.o obj/character_factory.o obj/character.o obj/cell.o obj/logger.o
	$(CC) $(CPPFLAGS2) $(CFLAGS) $^ -o $@

obj/character_factory_main.o: src/character/main.c include/character_factory.h include/logger.h include/memory.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/character_factory.o: src/character/character_factory.c include/character_factory.h include/logger.h include/memory.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/character.o: src/character/character.c include/character.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/cell.o: src/cell.c include/cell.h include/character.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# CLEANING
# ----------------------------------------------------------------------------
run: all
	./bin/main_program

clean:
	rm obj/*.o

distclean: clean
	rm bin/*

