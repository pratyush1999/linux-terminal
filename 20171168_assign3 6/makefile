myshell : main.o commands.o getDirectory.o getInput.o parse.o 
		gcc $^ -o shell

commands.o : commands.c
	gcc -c commands.c

parse.o : parse.c
	gcc -c parse.c

getDirectory.o : getDirectory.c
	gcc -c getDirectory.c

getInput.o : getInput.c
	gcc -c getInput.c

main.o : main.c
	gcc -c main.c

		
