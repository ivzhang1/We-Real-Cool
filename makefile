all: client database

client: client.o commons.o login.o
	gcc -o client client.o commons.o login.o

client.o: client.c client.h
	gcc -c client.c

database: database.o commons.o
	gcc -o database database.o commons.o

database.o: database.c database.h
	gcc -c database.c

login.o: login.c login.h
	gcc -c login.c

commons.o: commons.c commons.h
	gcc -c commons.c

clean:
	rm *.o
