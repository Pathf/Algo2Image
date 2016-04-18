all:
	#-L. pour la compilation
	g++ -std=c++11 quadtree.cpp -c -L.
	g++ -std=c++11 test_quadtree.cpp quadtree.o -o test.exe -L. -llodepng

testI:
	g++ -std=c++11 quadtree.cpp -c -L.
	g++ -std=c++11 test_imagepng.cpp quadtree.o -o test.exe -L. -llodepng
