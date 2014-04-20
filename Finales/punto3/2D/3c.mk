all: evolve.dat

evolve.dat: evolve.x data.dat
	./evolve.x 1000 5 data.dat
	python plots.py evolve.dat
	rm -f *.x
	rm -f *.dat
	echo Clean-Done

evolve.x: IC.x evolve.c
	cc evolve.c -o evolve.x -lm

data.dat: IC.x
	./IC.x 0 0 0 0 300 500000000 20 56 data.dat

IC.x:	IC.c
	cc IC.c -o IC.x -lm

clean:

