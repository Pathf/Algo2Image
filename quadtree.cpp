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
	_taille = 0;
	_racine.pere = nullptr;
	for(auto & f : _racine.fils)
		f = nullptr;
}

//------------------------------------------------------------------------------
QuadTree::~QuadTree(){	
	destructeur(&_racine);
}

//------------------------------------------------------------------------------
void QuadTree::afficher() const
{
    if ( _taille == 0 ){
        cout << "Quadtree vide." << endl;
    } else {
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

	tabCpt[_taille];

	for(int i = 0; i < _taille; i++)
		tabCpt[i] = 0;
	
	importer_rec(&_racine, _taille - 1, img); // _taille -1 pour s'arretter au bon moment
}

//------------------------------------------------------------------------------
ImagePNG QuadTree::exporter() const
{
	// reconstitution de la taille d'un coté
	unsigned coteImage = 1;
	for(int i = 0; i < _taille; i++)
		coteImage *= 2;

    ImagePNG img(coteImage, coteImage);

	exporter_rec(&_racine, img, 0, 0, coteImage);

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
void QuadTree::destructeur(Noeud * ptr){
	for(auto f : ptr->fils)
		if(f)
			destructeur(f);

	for(auto f : ptr->fils)
		delete f;
}

//------------------------------------------------------------------------------
void QuadTree::importer_rec(Noeud * ptr, unsigned taille, const ImagePNG & img){
	unsigned x, y;
	unsigned color = 0;
	unsigned larg = img.largeur();
	if(taille > 0){
		for(int i = 0; i < 4; i++){
        	ptr->fils[i] = new Noeud;
			ptr->fils[i]->pere = ptr;
		}

		for(auto f : ptr->fils){
			importer_rec(f, taille - 1, img);
			tabCpt[_taille - taille] += 1;		 		
		}

		for(int i = 0; i < 4; i++){	 // Pour la valeur de chaque fils
			color += ((ptr->fils[i]->rvb.R + ptr->fils[i]->rvb.V + ptr->fils[i]->rvb.B) / 3);
		}

		// Pour la valeur du pere (moyenne des fils)
		ptr->rvb.R = ptr->rvb.V = ptr->rvb.B = color /4;
		color = 0;
	} else {
		for(int i = 0; i < 4; i++){
	        ptr->fils[i] = new Noeud;
			for(int j = 0; j < 4; j++)
				ptr->fils[i]->fils[j] = nullptr; // Pour la destruction et l'exportation
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
			larg = img.largeur();
			ptr->fils[i]->rvb = img.lirePixel(x,y); // A VERIFF G UN DOUTE SUR LE POINT DE .rvb
		}
	}
	taille++; // car on revient un pas en arriere dans l'arbre
}

//------------------------------------------------------------------------------
void QuadTree::exporter_rec(const Noeud* ptr, ImagePNG & img, unsigned x, unsigned y, unsigned taille) const
{
	if(ptr->fils[0] != NULL){
			for(unsigned i = 0; i < taille ; i++){
				for(unsigned j = 0; j < taille; j++){
					img.ecrirePixel(x + i, y + j, ptr->rvb);
				}
			}
	} else {
		exporter_rec(ptr->fils[0], img, x, y, taille/2);
		exporter_rec(ptr->fils[1], img, x + taille/2, y, taille/2);
		exporter_rec(ptr->fils[2], img, x, y + taille/2, taille/2);
		exporter_rec(ptr->fils[3], img, x + taille/2, y + taille/2, taille/2);
	}
}
