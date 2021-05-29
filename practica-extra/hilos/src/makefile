CC=gcc
CFLAGS=-g -Wall #-Wextra
CLIBS=-pthread
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
EXECS=$(SOURCES:%.c=%)

.PHONY: all gitignore
all: $(EXECS)
# all: $(OBJECTS) $(EXECS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(CLIBS)

# .c.o:
# 	$(CC) $(CFLAGS) -c $< -o $@ $(CLIBS)
# .o.:
# 	$(CC) $^ $(CFLAGS) -o $@ $(CLIBS)

.PHONY: gitignore
gitignore:
	echo $(EXECS) > .gitignore

.PHONY: clean
clean:
	-@ rm -f *.o
	-@ rm $(EXECS)
