all: cardGame.exe

cardGame.exe: cardGame.o functions.o
	gcc cardGame.o functions.o -o cardGame.exe

cardGame.o: cardGame.c functions.h
	gcc -c cardGame.c

functions.o: functions.c functions.h
	gcc -c functions.c

clean:
	del /Q /F *.o cardGame.exe cardGame.o functions.o

# rm -f cardGame.exe , exe = out - за Linux