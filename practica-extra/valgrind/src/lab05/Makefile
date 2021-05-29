CFLAGS = -Wall -g
CC = gcc
EXES = memleak_example memleak memviolation_array memviolation

all: $(EXES)

clean:
	rm -f $(EXES)

submit: worksheet.txt memleak.c memviolation.c
	~/bin/submit -c=IC221 -p=lab4_memory $^

test: test.sh memleak memviolation
	./test.sh

.PHONY: all clean submit test
