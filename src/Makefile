all:
	@gcc -g professor_challenge.c -o exec -fopenmp -lm
run:
	@./exec
valgrind:
	@valgrind --leak-check=full --show-leak-kinds=all ./exec
