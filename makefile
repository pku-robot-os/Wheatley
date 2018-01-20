
build: 
	gcc -o manager service_management.c server.c
	gcc -o mic mic.c -I/usr/include/python2.7 -lpython2.7
	rm *.o

clean:
	rm *.o 
