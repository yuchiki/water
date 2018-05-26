water: water.cpp
	g++ -std=c++17 -o water water.cpp

.PHONY: clean
clean:
	- rm -f water