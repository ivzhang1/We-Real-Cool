all: client database account_manager

client: client.o commons.o login.o
	gcc -o client client.o commons.o login.o

account_manager: account_manager.o commons.o
	gcc -o account_manager account_manager.o commons.o

client.o: client.c client.h
	gcc -c client.c

database: database.o commons.o table.o al.o qs.o
	gcc -o database database.o commons.o table.o al.o qs.o

database.o: database.c database.h
	gcc -c database.c

account_manager.o: account_manager.c account_manager.h
	gcc -c account_manager.c

login.o: login.c login.h
	gcc -c login.c

table.o: table.c table.h
	gcc -c table.c

al.o: al.c al.h
	gcc -c al.c

qs.o: qs.c qs.h

commons.o: commons.c commons.h
	gcc -c commons.c

clean:
	rm *.o
	rm *.h~
