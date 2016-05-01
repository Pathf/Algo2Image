all:
	make CcQuad CcTest_q CoProg RProg

CcQuad:
	g++ -std=c++11 quadtree.cpp -c -L.

CcTest_q:
	g++ -std=c++11 test_quadtree.cpp -c -L.

CoProg:
	g++ -std=c++11 test_quadtree.o quadtree.o -o prog -L. -llodepng
	
RProg:
	./prog

#####################################################################
#		Utilisation du Makefile :									#
# ----------------------------------------------------------------- #
#  all : Permet de tout executer										#
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
