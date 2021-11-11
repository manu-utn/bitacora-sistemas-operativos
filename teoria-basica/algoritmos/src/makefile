RM = rm -vrf

all: clean run

run:
	@gcc $(f).c -g -l commons -o $(f) && ./$(f)
#	@gcc $(f).c -g -l commons -o $(f) && valgrind ./$(f)

clean: cleanout
	@$(RM) vgcore.*

cleanout:
	@$(RM) a.out
