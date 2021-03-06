all: evolve.dat

evolve.dat: evolve.x data1.dat data2.dat
	cat data1.dat data2.dat > dataFinal.dat
	./evolve.x 10000 5 dataFinal.dat
	python plots2.py evolve_d_1.dat evolve_d_2.dat evolve_d_3.dat evolve_d_4.dat evolve_d_5.dat
	rm -f *.x
	rm -f *.dat
	echo Clean-Done

evolve.x: IC.x evolve.c
	cc evolve.c -o evolve.x -lm

data1.dat: IC.x
	./IC.x 0 0 0 0 100000 100000000000 100 56 data1.dat

data2.dat: IC.x
	./IC.x 150 200 0 -100 100000 100000000000 100 82 data2.dat

IC.x:	IC.c
	cc IC.c -o IC.x -lm

clean:

