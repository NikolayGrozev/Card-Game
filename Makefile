all: runEncrypt cardGame.exe

encrypt.exe: encrypt.c
	gcc encrypt.c -o encrypt.exe

cardGame.exe: cardGame.c
	gcc cardGame.c -o cardGame.exe

clean:
	del /Q /F encrypt.exe cardGame.exe

# rm -f encrypt.exe cardGame.exe , exe = out - за Linux