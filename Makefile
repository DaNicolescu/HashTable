build: hash.o functions.o
	gcc hash.o functions.o -o hash_table
hash.o: hash.c
	gcc -c hash.c
functions.o: functions.c
	gcc -c functions.c
clean:
	rm -f *.o hash_table	
