all:
	gcc -g main.c  tube.c stack.c color.c load.c
clean:
	rm -f a.out
