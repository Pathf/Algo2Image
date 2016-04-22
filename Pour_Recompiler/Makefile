all:
	g++ -std=c++11 -c imagepng.cpp
	g++ -std=c++11 -c lodepng.cpp
	ar cruvs liblodepng.a imagepng.o lodepng.o

# sinon avec l'option gpp

gpp:
	g++ --ansi -pedantic -Wfatal-errors -Wconversion -Wredundant-decls -Wshadow -Wall -Wextrastd=c++11 -c imagepng.cpp
	g++ -ansi -pedantic -Wfatal-errors -Wconversion -Wredundant-decls -Wshadow -Wall -Wextra -std=c++11 -c lodepng.cpp
	ar cruvs liblodepng.a imagepng.o lodepng.o
