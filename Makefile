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
bin/monitor: src/monitor/main.o src/monitor/monitor.o src/monitor/monitor_common.o src/common/logger.o src/character/character.o src/cell.o
	$(CC) $^ -o $@ $(LDFLAGS)

src/monitor/main.o: src/monitor/main.c include/monitor.h include/monitor_common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

src/monitor/monitor.o: src/monitor/monitor.c include/monitor.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

src/monitor/monitor_common.o: src/monitor/monitor_common.c include/monitor_common.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c



# ----------------------------------------------------------------------------
# COMMON OBJECTS FILES
# ----------------------------------------------------------------------------

src/common/logger.o: src/common/logger.c include/logger.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c


# ----------------------------------------------------------------------------
# CHARACTER OBJECTS FILES
# ----------------------------------------------------------------------------

src/character/character.o: src/character/character.c include/character.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

src/cell/cell.o: src/cell/cell.c include/cell.h include/character.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@ -c

# ----------------------------------------------------------------------------
# CLEANING
# ----------------------------------------------------------------------------
clean:
	rm src/monitor/*.o src/common/*.o

distclean: clean
	rm bin/monitor

