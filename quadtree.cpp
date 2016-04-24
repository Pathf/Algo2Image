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
	_nbNoeud = 0;	
	creationQuadTree(ptr, _taille - 1, 0, 0); // _taille -1 pour s'arretter au bon moment
}

//------------------------------------------------------------------------------
Noeud* QuadTree::creationQuadTree(Noeud * ptr, unsigned taille, unsigned posX, unsigned posY){
	unsigned x, y;
	if(taille > 0){
		for(int i = 0; i < 4; i++){
        	ptr->fils[i] = new Noeud; // Au cas ou
			ptr->fils[i]->pere = ptr;
		}
		ptr = creationQuadTree(ptr->fils[0], taille - 1, cpt);
		ptr = creationQuadTree(ptr->fils[1], taille - 1, cpt);
		ptr = creationQuadTree(ptr->fils[2], taille - 1, cpt);
		ptr = creationQuadTree(ptr->fils[3], taille - 1, cpt);
	}

	for(int i = 0; i < 4; i++){
        ptr->fils[i] = new Noeud; // Au cas ou
		ptr->fils[i]->pere = ptr;
		
		if(i == 0){ 		// coordonnée : x = 0 et y = 0
				x = posX;
				y = posY;
		} else if(i == 1){	// coordonnée : x = 1 et y = 0
				x = posX + 1;
				y = posY;
		} else if(i == 2){	// coordonnée : x = 0 et y = 1
				x = posX;
				y = posY + 1;
		} else {			// coordonnée : x = 1 et y = 1
				x = posX + 1;
				y = posY + 1;
		}
		ptr->fils[i].rvb = image.lirePixel(x,y);
	}


// A FINIR :
	if(_taille > 1){
		_nbNoeud++;
	
		if(_nbNoeud % 2 == 0){
			posY += 2;
		} else {
			posX += 2;
		}
	}
	
	taille++; // car on revient un pas en arriere dans l'arbre
	return ptr->pere;
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
