#include <bitset>
#include <iostream>
#include <cassert>
#include <cmath>
#include "quadtree.hpp"
#include <vector>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Interface publique
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
QuadTree::QuadTree(){
	// Initialise toute les pointeurs du noeud à nullptr et la taille à 0
	_taille = 0;
	_racine.pere = nullptr;
	for(auto f : _racine.fils)
		f = nullptr;
}

//------------------------------------------------------------------------------
QuadTree::~QuadTree(){
	// Fait appel à la méthode récursive destructeur();
	destructeur(&_racine);
}

//------------------------------------------------------------------------------
void QuadTree::afficher() const {
	// Fait appel à la méthode récursive affiche_rec(); pour afficher l'arbre dans sa globalité (si la _taille est différente de 0)
    if(_taille == 0)
        cout << "Quadtree vide." << endl;
    else
        afficher_rec(&_racine);
}

//------------------------------------------------------------------------------
void QuadTree::importer(const ImagePNG & img){ // A VERIFIER PAR RAPPORT A UN ARBRE PLUS ANCIEN
	//initialise la taille à 0 :	
	_taille = 0;
	// Creation de la taille de l'arbre à partir de la largeur de l'image
	unsigned larg = img.largeur();	
	while(larg != 1){
		larg /= 2;
		_taille++;
	}

	// Appelle de la methode récursive importer_rec(); pour créer tous l'arbre à partir d'une image
	importer_rec(&_racine, img.largeur(), img, 0, 0);
}

//------------------------------------------------------------------------------
ImagePNG QuadTree::exporter() const{
	// Reconstitution de la taille d'un coté de l'image d'origine
	unsigned coteImage = 1;
	for(int i = 0; i < _taille; i++)
		coteImage *= 2;

	// Création d'une image blanche avec le hauteur et largeur de l'image de base
    ImagePNG img(coteImage, coteImage);

	// Utilisation de la méthode récursive exporter_rec(); pour créer dans sa totalité l'image
	exporter_rec(&_racine, coteImage, img, 0, 0);

	// Et on retourne cette image créé
    return img;
}

//------------------------------------------------------------------------------
void QuadTree::compressionDelta(unsigned delta){
	// Verifie que le delta est correcte (une couleur ne peut pas etre superieur à 255)
	assert(delta < 255);

	// Reconstitution de la taille d'un coté de l'image
	unsigned tailleI = 1;
	for(int i = 0; i < _taille; i++)
		tailleI *= 2;

	// Si delta est égal à 0 alors simple appel de la méthode récursive compressionSansPerte_rec(); qui evite des calcules inutile
	// Sinon appel de la methode récursive compressionDelta_rec();
	if(delta == 0)
		compressionSansPerte_rec(&_racine, tailleI);
	else
		compressionDelta_rec(&_racine, tailleI, delta);
}

//------------------------------------------------------------------------------
void QuadTree::compressionPhi(unsigned phi){ // A FINIR
	// Vérification car suprimmer la racine n'a pas de sens
	assert(phi > 0);

	if(phi == 1)
		destructeur(&_racine);
		
	if(phi == 4)
		for(auto f : _racine.fils)
			destructeur(f);
	/*else {
	// Reconstitution de la taille d'un coté de l'image
	unsigned tailleI = 1;
	for(int i=0; i<_taille; i++)
		tailleI *= 2;
	// Reconstitution du nombre de feuille au total
	unsigned nbfeuille = tailleI * tailleI;

	// Si le nombre de feuille est superieur à phi alors on utilise la methode récursive compressionPh_rec();
	// sinon on ne fait rien car il est deja au norme demandé
	if(nbfeuille > phi)
		compressionPhi_rec(&_racine, phi, tailleI, nbfeuille);
	}*/
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Fonctions facilitatrices (privées)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/*static*/ QuadTree::bit QuadTree::kiemeBit(unsigned n, unsigned k){
    bit b = 0;
    if(k < 31){
        std::bitset<32> bits(n);
        b = bits[k];
    }

    return b;    
}

//------------------------------------------------------------------------------
/*static*/ void QuadTree::afficher_rec(const Noeud * n, string tabs/*=""*/){
	// Affichage du noeud
    if (n != nullptr){
        cout << tabs << "- " << n << " de " << n->pere << " vers ";
        for (auto f : n->fils)
            cout << f << " ";

        cout << "RVB=" << n->rvb << endl;
        // Et de ses fils récursivement
        for (auto f : n->fils)
            afficher_rec(f,tabs+"   ");
    }
}

//------------------------------------------------------------------------------
void QuadTree::destructeur(Noeud * ptr){
	// On fait un appelle jusqu'a tombe sur les feuilles
	for(auto f : ptr->fils)
		if(f) // != nullptr
			destructeur(f);

	// On supprime les fils au fur et à mesure
	for(int j=0; j < 4; j++){
		delete ptr->fils[j];
		ptr->fils[j] = nullptr;
	}
}

//------------------------------------------------------------------------------
void QuadTree::importer_rec(Noeud * ptr, unsigned taille, const ImagePNG & img, unsigned x, unsigned y){
	// Creation des Noeud au fur et a mesure des appelle
	for(int i = 0; i < 4; i++){
		ptr->fils[i] = new Noeud;
		ptr->fils[i]->pere = ptr;
		for(int j = 0; j < 4; j++)
			ptr->fils[i]->fils[j] = nullptr;
	}

	// On fait des appelle tant que l'arbre comporte des portions superieur a 4 pixels
	// (2 etant la largeur d'un des cote de l'image donc 2*2 = 4)
	if(taille > 2){
		importer_rec(ptr->fils[0], taille/2, img, x, y);	
		importer_rec(ptr->fils[1], taille/2, img, x, y + (taille/2));
		importer_rec(ptr->fils[2], taille/2, img, x + (taille/2), y);		
		importer_rec(ptr->fils[3], taille/2, img, x + (taille/2), y + (taille/2));
	} else {
		// Sinon on ecrit lit la couleur pour la feuille au pixel correspondant
		ptr->fils[0]->rvb = img.lirePixel(x,y);
		ptr->fils[1]->rvb = img.lirePixel(x,y+1);
		ptr->fils[2]->rvb = img.lirePixel(x+1,y);
		ptr->fils[3]->rvb = img.lirePixel(x+1,y+1);
	}

	// On fait la moyenne de la couleur pour tout les Noeuds qui ne sont pas feuille
	vector<Couleur> vectTmp;
	for(auto f : ptr->fils)
		vectTmp.push_back(f->rvb);

	ptr->rvb = moyenne(vectTmp);
}

//------------------------------------------------------------------------------
void QuadTree::exporter_rec(const Noeud* ptr,  unsigned taille, ImagePNG & img, unsigned x, unsigned y) const {
	// Si le pointeur vise une feuille alors on ecrit les pixels correspondant
	// Sinon on réappel la methode récursive exporter_rec(); jusqu'a ce qu'on obtienne une feuille
	if(ptr->fils[0] == nullptr)
			for(unsigned i = 0; i < taille ; i++)
				for(unsigned j = 0; j < taille; j++)
					img.ecrirePixel(x + i, y + j, ptr->rvb);
	else {
		exporter_rec(ptr->fils[0], taille/2, img, x, y);
		exporter_rec(ptr->fils[1], taille/2, img, x, y + taille/2);
		exporter_rec(ptr->fils[2], taille/2, img, x + taille/2, y);
		exporter_rec(ptr->fils[3], taille/2, img, x + taille/2, y + taille/2);
	}
}

//------------------------------------------------------------------------------
void QuadTree::compressionSansPerte_rec(Noeud* ptr, unsigned taille){
	// appelle recursif : cas d'arret quand on obtient une portion de 4 pixel
	if(taille > 2)
		for(auto f : ptr->fils)
			compressionSansPerte_rec(f, taille/2);	

	// verifie si la couleur est pareil dans le noeud et dans un des fils si oui alors suprime les fils
	if(ptr->rvb.R == ptr->fils[0]->rvb.R && ptr->rvb.V == ptr->fils[0]->rvb.V && ptr->rvb.B == ptr->fils[0]->rvb.B)
		destructeur(ptr);
}

//------------------------------------------------------------------------------
void QuadTree::compressionDelta_rec(Noeud* ptr, unsigned taille, unsigned delta){
	// appelle recursif : cas d'arret quand on obtient une portion de 4 pixel
	if(taille > 2)
		for(auto f : ptr->fils)		
			compressionDelta_rec(f, taille/2, delta);

	// Regarde si les fils du noeud sont des feuilles
	bool estFeuille = true;
	for(auto f : ptr->fils)
		for(auto f2 : f->fils)
			if(f2) //  != nullptr
				estFeuille = false;

	// Permet de ne faire que les cas ou les fils sont des feuilles
	if(estFeuille){
		unsigned maxLumFils = 0;
		for(auto f : ptr->fils)
			if(diff_lum(f->rvb, ptr->rvb) > maxLumFils)
				maxLumFils = diff_lum(f->rvb, ptr->rvb);

		if(maxLumFils <= delta)
			destructeur(ptr);
	}
}

//------------------------------------------------------------------------------
void QuadTree::compressionPhi_rec(Noeud* ptr, unsigned phi, unsigned nbfeuille, unsigned taille){ // A FINIR
/*	if(taille > 2){
		compressionPhi_rec(ptr->fils[0], phi, nbfeuille, taille);
	} //else {
	vector<pair<unsigned, Noeud*>> vecStockTmp;
	if(nbfeuille > phi){
//		compressionPhi_rec();

		// Pour la difference de luminence
		unsigned maxLumFils = 0;
		for(int i=0; i<4; i++)
			if(diff_lum(ptr->fils[i]->rvb, ptr->rvb) > maxLumFils)
				maxLumFils = diff_lum(ptr->fils[i]->rvb, ptr->rvb);
		
	}
*/}
/*
void QuadTree::stockLumMax(){

}*/
