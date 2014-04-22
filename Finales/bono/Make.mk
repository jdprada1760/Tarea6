all: 3body.data

3body.data : grav_colapso.x
	./grav_colapso.x 0.001 53
	python plots.py
	rm -f *.x
	rm -f *.data
grav_colapso.x : grav_colapso.c
	cc grav_colapso.c -o grav_colapso.x -lm 

clean :
	rm -f *.x
	rm -f *.data
