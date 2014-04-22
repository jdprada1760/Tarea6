all: plot_solutions.py

plot_solution.py : datass.o a.o 
	python plot_solutions.py

datass.o : datass.dat
	./a.o

a.o : a.out
	cc -lm volterra_lotka.c

clean :
	rm -f *.o