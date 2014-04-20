all: 3body.data

3body.data: Final3Bodies_2.0.x
	./Final3Bodies_2.0.x 0.01
	python plots.py 3body.data
	rm -f *.x
	rm -f *.data
	echo Clean-Done

Final3Bodies_2.0.x: Final3Bodies_2.0.c
	cc  Final3Bodies_2.0.c -o Final3Bodies_2.0.x -lm

clean:

