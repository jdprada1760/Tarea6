all: datass.dat

datass.dat : volterra_lotka.x
	./volterra_lotka.x 30 20 0.0001
	python plot_solutions.py
	rm -f *.x
	rm -f *.dat
volterra_lotka.x : volterra_lotka.c
	cc volterra_lotka.c -o volterra_lotka.x -lm 

clean :
	rm -f *.o
