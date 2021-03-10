all:
	gcc -g main.c
clean:
	rm -f a.out

tags:
	ctags -R . 

