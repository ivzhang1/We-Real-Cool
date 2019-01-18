all: client database

client: client.o commons.o
	gcc -o client client.o commons.o

client.o: client.c client.h
	gcc -c client.c

database: database.o commons.o table.o
	gcc -o database database.o commons.o table.o

database.o: database.c database.h
	gcc -c database.c

table.o: table.c table.h
	gcc -c table.c

commons.o: commons.c commons.h
	gcc -c commons.c

clean:
	rm *.o
