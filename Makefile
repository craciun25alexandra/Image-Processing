build: tema3.c
		gcc tema3.c -o bmp -Wall
run: build
		./bmp
clean:
		rm -f tema3 tema3.o