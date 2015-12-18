CC = g++
CFLAG = -g -fPIC -D_FILELINE

server: server.c
	gcc -g server.c -o sandboxServer -pthread

clean:
	rm -f *.o
	rm -f *.bak
	rm -f sandboxServer