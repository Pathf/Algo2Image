all:
	make CcQuad CcTestU CoProg RProg

CcQuad:
	g++ -std=c++11 quadtree.cpp -c -L.

CcTestU:
	g++ -std=c++11 testUnitaires.cpp -c -L.

CoProg:
	g++ -std=c++11 testUnitaires.o quadtree.o -o prog -L. -llodepng
	
RProg:
	./prog

# Suppression du prog, des .o et des images
Sall:
	make all SCc SProg SImg STild
	
SCc:
	rm quadtree.o testUnitaires.o
	
SProg:
	rm prog

SImg:
	rm zip-d-128-gnu.png zip-d-512-books.png zip-d-2048-earth.png

STild:
	sh SuppFauxFich.sh

#####################################################################
#		Utilisation du Makefile :									#
# ----------------------------------------------------------------- #
#  all : Permet de tout executer									#
#																	#
#  CcQuad : Permet de compiler vers un .o quadtree.cpp				#
#																	#
#  CcTest_q: Permet de compiler vers un .o test_quadtree.cpp		#
#																	#
#  CoProg : Permet de compiler l'ensemble dans un même programme	#
#																	#
#  RProg : Permet d'executer le programme qui a été compiler		#
#																	#
#####################################################################

#####################################################################
#		Petit expilcation sur la construction du Makefile :			#
# ----------------------------------------------------------------- #
#  C : pour compilation												#
#  Cc : pour la compilation vers .o									#
#  Co : pour la compilation vers un programme						#
#  R : pour l'execution d'un prog									#
#																	#
#####################################################################
#		Option de compilation :										#
#-------------------------------------------------------------------#
#  -L. pour la compilation											#
#  -llodepng : pour appeler la bibliothèque correspondante			#
#																	#																#
#####################################################################
