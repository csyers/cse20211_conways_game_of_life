all: life

life: life.c
	gcc life.c -o life

.PHONY: clean
clean:
	-rm life
