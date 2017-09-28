all:
	gcc -g no_wait.c -o exec -fopenmp -lm
run:
	./exec
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./exec
