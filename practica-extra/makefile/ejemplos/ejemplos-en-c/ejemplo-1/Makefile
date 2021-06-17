OBJS := $(wildcard *.o)
MEMCHECK_LOG = memcheck.txt
PROGRAMA = unprograma

.PHONY: all
all: clean unprograma

$(PROGRAMA): main.c player.o
	gcc -o $@ main.c player.o -g -Wall

player.o: player.c
	gcc -c player.c -g -Wall

# valgrind
memcheck:
	valgrind --tool=memcheck --leak-check=full \
	--show-leak-kinds=all --track-origins=yes \
	--log-file=$(MEMCHECK_LOG) ./$(PROGRAMA)

.PHONY: clean
clean:
	rm -rf $(OBJS)
	rm -rf $(PROGRAMA)
	rm -rf $(MEMCHECK_LOG)
