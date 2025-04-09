CC = gcc
CFLAGS = -O2 -march=native -ffast-math
LIBS = -lm
all :
	$(CC) -o basins main.c newton.c write_ppm.c $(CFLAGS) $(LIBS)



.PHONY : clean
clean :
	rm -f *.o

