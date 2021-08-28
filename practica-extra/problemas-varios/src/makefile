CC=gcc
CFLAGS=-g #-Wall #-Wextra
CLIBS=-pthread

.PHONY: all
all: compile run clean

.PHONY: run
run:
	@./$(NAME)

.PHONE: compile
compile:
	@$(CC) $(CLIBS) $(CFLAGS) -o $(NAME) $(NAME).c

.PHONY: clean
clean:
	@rm ./$(NAME)
