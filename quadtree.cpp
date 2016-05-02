#include <bitset>
#include <iostream>
#include <cassert>
#include <cmath>
#include <math.h>
#include <vector>		// std::vector
#include <utility>		// std::pair
#include <algorithm>    // std::sort
#include "quadtree.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Interface publique
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
QuadTree::QuadTree(){
	// Initialise tous les pointeurs du noeud à nullptr et la taille à 0
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
	_taille = img.largeur();

	// Appel de la methode récursive importer_rec(); pour créer tout l'arbre à partir d'une image
	importer_rec(&_racine, _taille, img, 0, 0);
}

//------------------------------------------------------------------------------
ImagePNG QuadTree::exporter() const{
	// Création d'une image blanche avec le hauteur et largeur de l'image de base
    ImagePNG img(_taille, _taille);

	// Utilisation de la méthode récursive exporter_rec(); pour créer dans sa totalité l'image
	exporter_rec(&_racine, _taille, img, 0, 0);

	// Et on retourne cette image créée
    return img;
}

//------------------------------------------------------------------------------
void QuadTree::compressionDelta(unsigned delta){
	// Verifie que le delta est correct (une couleur ne peut pas etre superieure à 255)
	assert(delta < 255);

	// Si delta est égal à 0 alors simple appel de la méthode récursive compressionSansPerte_rec(); qui evite des calculs inutiles
	// Sinon appel de la methode récursive compressionDelta_rec();
	if(delta == 0)
		compressionSansPerte_rec(&_racine, _taille);
	else
		compressionDelta_rec(&_racine, _taille, delta);
}

//------------------------------------------------------------------------------
void QuadTree::compressionPhi(unsigned phi){
	// Vérification car supprimer la racine n'a pas de sens
	assert(phi > 0);

	if(phi == 1)
		destructeur(&_racine);
		
	if(phi == 4)
		for(auto f : _racine.fils)
			destructeur(f);
	
	if(phi > 4){
		unsigned taille = _taille;
		// Reconstitution du nombre de feuille au total
		unsigned nbfeuille = _taille * _taille;

		vector<pair<unsigned, Noeud*>> vecStockTmp;
		stockLumMax(&_racine, _taille, &vecStockTmp);

		// sort: fonction de tri du vecteur en ordre décroissant, en utilisant une règle de comparaison
		// des éléments pairs définie sur les différences de luminosité
		std::sort(vecStockTmp.begin(), vecStockTmp.end(), [] (const pair<unsigned, Noeud*>& firstElem, const pair<unsigned, Noeud*>& secondElem) { return firstElem.first > secondElem.first; });

		// Si le nombre de feuille est superieur à phi alors on utilise la methode récursive compressionPh_rec();
		// sinon on ne fait rien car il est deja au norme demandé		
		while(nbfeuille > phi){
			Noeud * ptr = vecStockTmp.at(vecStockTmp.size() -1).second;
			destructeur(ptr);
			vecStockTmp.pop_back();

			nbfeuille -= 3;

			if(vecStockTmp.size() == 0){
				taille /= 2;
				stockLumMax(&_racine, taille, &vecStockTmp);
				// sort: fonction de tri du vecteur en ordre décroissant, en utilisant une règle de comparaison
				// des éléments pairs définie sur les différences de luminosité
				std::sort(vecStockTmp.begin(), vecStockTmp.end(), [] (const pair<unsigned, Noeud*>& firstElem, const pair<unsigned, Noeud*>& secondElem) { return firstElem.first > secondElem.first; });
			}
		}
	}
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
	// On fait un appel jusqu'à tomber sur les feuilles
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
	// Creation des Noeud au fur et a mesure des appels
	for(int i = 0; i < 4; i++){
		ptr->fils[i] = new Noeud;
		ptr->fils[i]->pere = ptr;
		for(int j = 0; j < 4; j++)
			ptr->fils[i]->fils[j] = nullptr;
	}

	// On fait des appels tant que l'arbre comporte des portions superieures a 4 pixels
	// (2 etant la largeur d'un des cotes de l'image donc 2*2 = 4)
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
	// Sinon on réappelle la methode récursive exporter_rec(); jusqu'a ce qu'on obtienne une feuille
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
	// appel recursif : cas d'arret quand on obtient une portion de 4 pixel
	if(taille > 2)
		for(auto f : ptr->fils)
			compressionSansPerte_rec(f, taille/2);	

	// verifie si la couleur est identique dans le noeud et dans un des fils si oui alors supprime les fils
	if(ptr->rvb.R == ptr->fils[0]->rvb.R && ptr->rvb.V == ptr->fils[0]->rvb.V && ptr->rvb.B == ptr->fils[0]->rvb.B)
		destructeur(ptr);
}

//------------------------------------------------------------------------------
void QuadTree::compressionDelta_rec(Noeud* ptr, unsigned taille, unsigned delta){
	// appel recursif : cas d'arret quand on obtient une portion de 4 pixel
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
void QuadTree::stockLumMax(Noeud* ptr, unsigned taille, vector<pair<unsigned, Noeud*>> * vecStock){
	if(taille > 2){
		for(auto f : ptr->fils)
			stockLumMax(f, taille/2, vecStock);					
	} else {
		// Pour la difference de luminence
		unsigned maxLumFils = 0;
		for(auto f : ptr->fils)
			if(diff_lum(f->rvb, ptr->rvb) > maxLumFils)
				maxLumFils = diff_lum(f->rvb, ptr->rvb);
		vecStock->push_back(make_pair(maxLumFils,ptr));
	}
}
