all: evolve.dat

evolve.dat: evolve.x data1.dat data2.dat
	cat data1.dat data2.dat > dataFinal.dat
	./evolve.x 100 5 dataFinal.dat
	python plots2.py evolve.dat
	rm -f *.x
	rm -f *.dat
	echo Clean-Done

evolve.x: IC.x evolve.c
	cc evolve.c -o evolve.x -lm

data1.dat: IC.x
	./IC.x 0 0 0 0 100000 5000000000 20 56 data1.dat

data2.dat: IC.x
	./IC.x 40 50 0 -30 100000 50000000 20 82 data2.dat

IC.x:	IC.c
	cc IC.c -o IC.x -lm

clean:

