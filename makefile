all: user.o control.o
	gcc control.o -o control
	gcc user.o

user.o: user.c
	gcc -c user.c

control.o: control.c
	gcc -c control.c

setup:
	./control $(args)

run:
	./a.out

clean:
	rm *.o
	rm a.out
	rm control
