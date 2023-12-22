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

.PHONY: all clean distclean

all: bin/monitor

# ----------------------------------------------------------------------------
# MONITOR
# ----------------------------------------------------------------------------
bin/monitor: obj/timer/timer.o obj/common/memory.o obj/main.o obj/monitor/monitor.o obj/monitor/monitor_common.o obj/common/logger.o obj/character/character.o obj/cell.o
	$(CC) $^ -o $@ $(LDFLAGS)

obj/main.o: src/monitor/main.c include/monitor.h include/monitor_common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/monitor/monitor.o: src/monitor/monitor.c include/monitor.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/monitor/monitor_common.o: src/monitor/monitor_common.c include/monitor_common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c



# ----------------------------------------------------------------------------
# COMMON OBJECTS FILES
# ----------------------------------------------------------------------------

obj/common/logger.o: src/common/logger.c include/logger.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/common/memory.o: src/common/memory.c include/memory.h include/cell.h include/character.h include/common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# CHARACTER OBJECTS FILES
# ----------------------------------------------------------------------------

obj/character/character.o: src/character/character.c include/character.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

obj/cell.o: src/cell.c include/cell.h include/character.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

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
# CLEANING
# ----------------------------------------------------------------------------
run: all
	./bin/monitor

clean:
	rm obj/*.o

distclean: clean
	rm bin/monitor

