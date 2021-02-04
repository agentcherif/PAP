.PHONY: default clean

SOURCE	= $(wildcard *.c)
EXEC	= $(SOURCE:.c=)

CC	= gcc-10
CFLAGS	= -fopenmp -O3
LDFLAG	= -fopenmp
LDLIBS	= -lm

default: $(EXEC)

clean:
	rm -f $(EXEC)
