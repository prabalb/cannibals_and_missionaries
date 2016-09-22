OBJS = main.o cannMiss.o
CC = g++
DEBUG = -g -O3
CFLAGS = -std=c++11 -Wall -fPIC -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

runProg : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o runProg

main.o : main.cpp cannMiss.cpp cannMiss.h
	$(CC) $(CFLAGS) main.cpp

cannMiss.o : cannMiss.cpp cannMiss.h
	$(CC) $(CFLAGS) cannMiss.cpp

clean:	
	\rm *.o runProg
