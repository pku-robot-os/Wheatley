
build: 
	gcc dialog.c util.c pronounce/pronounce.c server.c ./libmsc.so -lasound -o dialog
	gcc mic.c util.c -lportaudio -lrt -lasound -ljack -lpthread -lportaudio -o mic
	gcc Wheatley.c util.c pronounce/pronounce.c ./libmsc.so -lasound -o Wheatley
	gcc service_management.c server.c -o manager
clean:
	rm *.o 
