OBJS := $(wildcard *.o)
MEMCHECK_LOG = memcheck.txt
PROGRAMA = $(SOURCE_BIN)/unprograma
SOURCE_PATH = source
SOURCE_BIN = bin

CC = gcc
CFLAGS = -g -Wall

.PHONY: all
all: clean unprograma

#$(SOURCE_PATH)/player/%.o: $(SOURCE_PATH)/player/%.c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

unprograma: $(SOURCE_PATH)/main.c $(SOURCE_PATH)/player/player.o
	$(CC) $(CFLAGS) $? -o $(PROGRAMA)
	@echo "Oe tío, hemos compilado tu programa :)"

# valgrind
memcheck:
	valgrind --tool=memcheck --leak-check=full \
	--show-leak-kinds=all --track-origins=yes \
	--log-file=$(MEMCHECK_LOG) $(PROGRAMA)

.PHONY: clean
clean:
	rm -rf $(OBJS)
	rm -rf $(PROGRAMA)
	rm -rf $(MEMCHECK_LOG)

.PHONY: run
run: ; @$(PROGRAMA)
