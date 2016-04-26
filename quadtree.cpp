#include <bitset>
#include <iostream>
#include <cassert>
#include <cmath>
#include "quadtree.hpp"

using namespace std;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Interface publique
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
QuadTree::QuadTree(){
	_taille = 0;
	_racine.pere = NULL;
	for(int i = 0; i < 3; i++)
		_racine.fils[i] = NULL;
}

//------------------------------------------------------------------------------
QuadTree::~QuadTree(){
	Noeud * ptr = &_racine;	
	supprToutPtr(ptr);
}

//------------------------------------------------------------------------------
void QuadTree::afficher() const
{
    if ( _taille == 0 )
    {
        cout << "Quadtree vide." << endl;
    }
    else
    {
        afficher_rec(&_racine);
    }
}

//------------------------------------------------------------------------------
void QuadTree::importer(const ImagePNG & img) // A FINIR
{
	unsigned larg = img.largeur();

	while(larg != 1){
		larg /= 2;
		_taille++; // incremente pour obtenir la taille de l'arbre 
	}
	
	Noeud * ptr = &_racine;

	unsigned tabCpt[_taille];

	for(int i = 0; i < _taille; i++)
		tabCpt[i] = 0;
	
	creationQuadTree(ptr, _taille - 1, tabCpt, img); // _taille -1 pour s'arretter au bon moment
}

//------------------------------------------------------------------------------
ImagePNG QuadTree::exporter() const
{
    ImagePNG img;
// À COMPLÉTER
    return img;
}

//------------------------------------------------------------------------------
void QuadTree::compressionDelta(unsigned delta)
{
// À COMPLÉTER
}

//------------------------------------------------------------------------------
void QuadTree::compressionPhi(unsigned phi)
{
// À COMPLÉTER
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Fonctions facilitatrices (privées)
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/*static*/ QuadTree::bit QuadTree::kiemeBit(unsigned n, unsigned k)
{
    
    bit b = 0;
    if ( k < 31 )
    {
        std::bitset<32> bits(n);
        b = bits[k];
    }
    return b;    
}

//------------------------------------------------------------------------------
/*static*/ void QuadTree::afficher_rec(const Noeud * n, string tabs/*=""*/)
{
    if (n != nullptr)
    {   // affichage du noeud
        cout << tabs << "- " << n << " de " << n->pere << " vers ";
        for (auto f : n->fils)
        {
            cout << f << " ";
        }
        cout << "RVB=" << n->rvb << endl;
        // et de ses fils récursivement
        for (auto f : n->fils)
        {
            afficher_rec(f,tabs+"   ");
        }
    }
}

//------------------------------------------------------------------------------
Noeud* QuadTree::supprToutPtr(Noeud * ptr){
	if(ptr->fils[0] != NULL){
		ptr = supprToutPtr(ptr->fils[0]);
	}

	if(ptr->fils[1] != NULL) {
		ptr = supprToutPtr(ptr->fils[1]);
	}
	
	if(ptr->fils[2] != NULL){
		ptr = supprToutPtr(ptr->fils[2]);
	}

	if(ptr->fils[3] != NULL){
		ptr = supprToutPtr(ptr->fils[3]);
	}

	for(int i = 0; i < 4; i++){
		delete ptr->fils[i];
        ptr->fils[i] = NULL; // Au cas ou
	}
	
	return ptr->pere;
}

//------------------------------------------------------------------------------
Noeud* QuadTree::creationQuadTree(Noeud * ptr, unsigned & taille, unsigned & tabCpt[], const ImagePNG & image){
	unsigned x, y;
	unsigned color = 0;
	unsigned larg = image.largeur();
	if(taille > 0){
		for(int i = 0; i < 4; i++){
        	ptr->fils[i] = new Noeud;
			ptr->fils[i]->pere = ptr;
		}
		ptr = creationQuadTree(ptr->fils[0], taille - 1, tabCpt);
		tabCpt[_taille - taille] += 1;		 		
		ptr = creationQuadTree(ptr->fils[1], taille - 1, tabCpt);
		tabCpt[_taille - taille] += 1;
		ptr = creationQuadTree(ptr->fils[2], taille - 1, tabCpt);
		tabCpt[_taille - taille] += 1;
		ptr = creationQuadTree(ptr->fils[3], taille - 1, tabCpt);
		tabCpt[_taille - taille] += 1;

		for(int i = 0; i < 4; i++){	 // Pour la valeur de chaque fils
			color += ((ptr->fils[i]->rvb.R + ptr->fils[i]->rvb.V + ptr->fils[i]->rvb.B) / 3);
		}
		// Pour la valeur du pere (moyenne des fils)
		ptr->rvb.R = ptr->rvb.V = ptr->rvb.B = color /4;
		color = 0;
	} else {
		for(int i = 0; i < 4; i++){
	        ptr->fils[i] = new Noeud; // Au cas ou
			ptr->fils[i]->pere = ptr;
			x = y = 0;
			for(int j = 0; j < _taille; j++){
				if(tabCpt[j] % 4 == 0){ 		// coordonnée : x = 0 et y = 0
					x = x + 0;
					y = y + 0;
				} else if(tabCpt[j] % 4 == 1){	// coordonnée : x = 1 et y = 0
					x = x + larg / 2;
					y = y + 0;
				} else if(tabCpt[j] % 4 == 2){	// coordonnée : x = 0 et y = 1
					x = x + 0;
					y = y + larg / 2;
				} else {			// coordonnée : x = 1 et y = 1
					x = x + larg / 2;
					y = y + larg / 2;
				}
				larg = larg / 2;
			}
			tabCpt[_taille - 1] += 1; 
			larg = image.largeur();
			ptr->fils[i]->rvb = image.lirePixel(x,y); // A VERIFF G UN DOUTE SUR LE POINT DE .rvb
		}
	}

	taille++; // car on revient un pas en arriere dans l'arbre
	return ptr->pere;
}
