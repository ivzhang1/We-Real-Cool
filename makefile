all: client database account_manager

client: client.o commons.o login.o
	gcc -o client client.o commons.o login.o

client.o: client.c client.h
	gcc -c client.c

database: database.o commons.o
	gcc -o database database.o commons.o

database.o: database.c database.h
	gcc -c database.c

account_manager: account_manager.o commons.o
	gcc -o account_manager account_manager.o commons.o

account_manager.o: account_manager.c account_manager.h
	gcc -c account_manager.c

login.o: login.c login.h
	gcc -c login.c

commons.o: commons.c commons.h
	gcc -c commons.c

clean:
	rm *.o
