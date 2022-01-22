CC=gcc
CFLAGS=-g #-Wall #-Wextra
CLIBS=-pthread
LIBRARIES=commons

.PHONY: all
all: compile run clean

.PHONY: run
run:
	@./$(NAME)

.PHONE: compile
compile:
	@$(CC) $(CLIBS) $(CFLAGS) -l=$(LIBRARIES) -o $(NAME) $(NAME).c

.PHONY: clean
clean:
	@rm ./$(NAME)
