LIBS=-lsqlite3
all:
	$(CC) main.c -o demo $(LIBS)

clean:
	rm -rf demo
