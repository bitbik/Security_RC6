run:rc6.o
		gcc rc6.o -o run
rc6.o:rc6.c
		gcc -c rc6.c -o rc6.o

clean:
		rm -rf *.o run