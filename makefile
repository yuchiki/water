water: water.c
	gcc -o water water.c

.PHONY: clean
clean:
	- rm -f water