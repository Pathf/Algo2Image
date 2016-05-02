#include "imagepng.hpp"
#include "quadtree.hpp"
#include <iostream>

using namespace std;

int main(){
	string fichier, nomF;
    ImagePNG originale,compressee;
    QuadTree arbre;
	int choix, delta, phi;
	
	cout << "\n######################################################" << endl;
	cout << "############## Lancement du programme : ##############" << endl;
	cout << "######################################################\n" << endl;

	cout << "Donner le nom de votre image : (le programme la cherchera dans pngs tout seul rentrer simplement le nom)" << endl;
	cout << "2.png  /  4.png  /  8.png  /  16.png  /  32-tux.png  /  64-tuxette.png  /  128-gnu.png  /  256-tux.png" << endl;
	cout << "256-trash.png  /  512-books.png  /  1024-cube.png  /  2048-earth.png  /  4096-palette.png  / 8192-snowflake.png" << endl;
	cout << "\nNom de l'image : ";
	cin >> fichier;
	nomF = fichier;
	fichier = "pngs/" + fichier;
	originale.charger(fichier);
	cout << "Fichier en importation (cela peut prendre du temps si l'image est de grosse taille)" << endl;
    arbre.importer(originale);
    
	cout << "------------------------------------------------------" << endl;
    cout << "Les options :" << endl;
    cout << "  1 - Affichage Original" << endl;
    cout << "  2 - Compression sans Perte" << endl;
    cout << "  3 - Compression delta" << endl;
    cout << "  4 - Compression phi" << endl;

    cout << "\nRentrez l'option que vous voulez affectez à votre image : ";
    cin >> choix;

    switch(choix){
    	case 1:
		    arbre.afficher();
    	break;

    	case 2:
			arbre.compressionDelta(0);
    		arbre.afficher();
    		compressee = arbre.exporter();
   			compressee.sauver("zip-d0-" + nomF);
    	break;

    	case 3:
    		cout << "------------------------------------------------------" << endl;
    		cout << "Choississez votre delta (delta < 255) : ";
			cin >> delta;
    		arbre.compressionDelta(delta);
    		arbre.afficher();
    		compressee = arbre.exporter();
   			compressee.sauver("zip-d-" + nomF);
	   	break;

   		case 4:
   			cout << "------------------------------------------------------" << endl;
   			cout << "Choississez votre phi (phi > 0) : ";
   			cin >> phi;
   			arbre.compressionPhi(phi);
		    arbre.afficher();
    		compressee = arbre.exporter();
   			compressee.sauver("zip-p-" + nomF);
   		break;
   	}

	cout << "\n######################################################" << endl;
	cout << "################# Fin du programme : #################" << endl;
	cout << "######################################################\n" << endl;
	
	return 0;             
}
