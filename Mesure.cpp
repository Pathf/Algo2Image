#include "imagepng.hpp"
#include "quadtree.hpp"
#include <iostream>
#include <cstdlib>		// srand() et rand()
#include <time.h>		// pour le temps
#include <fstream>		// Pour l'ecriture dans un fichier

using namespace std;

string const nomfichier("Rapport/resultat.txt");
/**
 * @var chrono
 * @brief chronomètre du programme
**/

clock_t chrono;

/**
 * @def START
 * @brief démarre le chronomètre
**/
#define START chrono=clock();

/**
 * @def STOP
 * @brief arrête le chronomètre
**/
#define STOP chrono=clock()-chrono;

/**
 * @def TEMPS
 * @brief donne le temps du chronomètre (après arrêt)
**/
#define TEMPS double(chrono)/(CLOCKS_PER_SEC*0.001)

/*********************************************************************/	
// LE MAIN
int main(){
	srand((unsigned int)time(0));
	
	std::ofstream flux(nomfichier.c_str(),std::ios::app);

	double tmp = 0.0;
	cout << "" << endl;

    ImagePNG originale,compressee;
    QuadTree arbre;
	int cpt = 0;
	
	do{

	if(cpt == 0){
		originale.charger("pngs/128-gnu.png");
		cout << "Fichier : 128-gnu.png" << endl;
		flux << "Fichier : 128-gnu.png" << endl;
	} else if(cpt == 1){
		originale.charger("pngs/512-books.png");
		cout << "Fichier : 512-books.png" << endl;
		flux << "Fichier : 512-books.png" << endl;
	} else {
		originale.charger("pngs/2048-earth.png");
		cout << "Fichier : 2048-earth.png" << endl;
		flux << "Fichier : 2048-earth.png" << endl;
	}
	

	START;
    arbre.importer(originale);
    STOP;

	flux << TEMPS - tmp << " => importer() de " << cpt << std::endl;
    
	START;
	arbre.compressionDelta(0);
	STOP;

	flux << TEMPS - tmp << " => compressionDelta(0) sans perte" << std::endl;
	
	START;
	compressee = arbre.exporter();
	STOP;
	
	flux << TEMPS - tmp << " => exporter() avec une compression sans perte" << std::endl;
	
	arbre.importer(originale);
	START;
	arbre.compressionDelta(50);
	STOP;

	flux << TEMPS - tmp << " => compressionDelta(50) delta = 50" << std::endl;
	
	START;
	compressee = arbre.exporter();
	STOP;
	
	flux << TEMPS - tmp << " => exporter() avec une compression delta 50" << std::endl;
	
	arbre.importer(originale);
	START;
	arbre.compressionDelta(200);
	STOP;
	
	flux << TEMPS - tmp << " => compressionDelta(200) delta = 200" << std::endl;
	
	START;
	compressee = arbre.exporter();
	STOP;
	
	flux << TEMPS - tmp << " => exporter() avec une compression delta 200" << std::endl;

	arbre.importer(originale);
	START;
	arbre.compressionPhi(50);
	STOP;

	flux << TEMPS - tmp << " => compressionPhi(50) phi = 50" << std::endl;
	
	START;
	compressee = arbre.exporter();
	STOP;
	
	flux << TEMPS - tmp << " => exporter() avec une compression phi 50" << std::endl;
	
	arbre.importer(originale);
	START;
	arbre.compressionDelta(200);
	STOP;
	
	flux << TEMPS - tmp << " => compressionPhi(200) phi = 200" << std::endl;
	
	START;
	compressee = arbre.exporter();
	STOP;
	
	flux << TEMPS - tmp << " => exporter() avec une compression phi 200\n" << std::endl;
	tmp = TEMPS;

	cout << "completed" << endl;
	cpt++;
	}while(cpt != 3);
	
	return 0;
}
