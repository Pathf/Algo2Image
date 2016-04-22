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
//	_racine.fils = NULL;
}

//------------------------------------------------------------------------------
QuadTree::~QuadTree(){
	Noeud * ptr = &_racine;	
	supprToutPtr(ptr);
}

//------------------------------------------------------------------------------
Noeud* QuadTree::supprToutPtr(Noeud * ptr){
	if(racine->fils[0] != NULL){
		ptr = supprToutPtr(ptr->fils[0]);
	}

	if(racine->fils[1] != NULL) {
		ptr = supprToutPtr(ptr->fils[1]);
	}
	
	if(racine->fils[2] != NULL){
		ptr = supprToutPtr(ptr->fils[2]);
	}

	if(racine->fils[3] != NULL){
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
    Couleur pixel;

	unsigned larg = img.largeur();

	

    for (unsigned x = 0; x < img.largeur(); x++)
    {
        for (unsigned y = 0; y < img.hauteur(); y++)
        {
            pixel = img.lirePixel(x,y);
            pixel.R = pixel.V = pixel.B = gris;
        }
    }
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
