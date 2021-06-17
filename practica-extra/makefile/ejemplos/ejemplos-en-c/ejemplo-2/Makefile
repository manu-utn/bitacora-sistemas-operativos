OBJS := $(wildcard *.o)
MEMCHECK_LOG = memcheck.txt
PROGRAMA = unprograma

CC = gcc
CFLAGS = -g -Wall

.PHONY: all
all: clean unprograma

$(PROGRAMA): main.c player.o
	$(CC) $^ $(CFLAGS) -o $@
	@echo "Oe tío, hemos compilado tu programa :)"

player.o: player.c
	$(CC) $(CFLAGS) -c $^

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

.PHONY: run
run: ; @./$(PROGRAMA)
