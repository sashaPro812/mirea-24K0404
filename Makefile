all: copy
copy: copy.c
	gcc -o copy copy.c
clean:
	rm -f copy