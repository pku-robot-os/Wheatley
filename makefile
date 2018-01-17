
build: 
	gcc -c *.c
	gcc -o manager service_management.o server.o
	rm *.o


clean:
	rm *.o 
