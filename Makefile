CC = g++ -std=c++17
CCFLAGS = -g

all: main.o Game.o Snake.o
	$(CC) $(CCFLAGS) -o Snake_Game main.o Game.o Snake.o -lncurses

clean:
	rm -f *.o Snake_Game

%.o : %.cpp %.h
	$(CC) $(CCFLAGS) -c $<

%.o : %.cpp
	$(CC) $(CCFLAGS) -c $<

% : %.cpp
	$(CC) $(CCFLAGS) -o $@ $<
