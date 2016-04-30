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
	for(int i=0; i < 4; i++)
		_racine.fils[i] = nullptr;
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
	_taille = 0;
	unsigned larg = img.largeur();	
	while(larg != 1){
		larg /= 2;
		_taille++; // incremente pour obtenir la taille de l'arbre 
	}
	importer_rec(&_racine, img.largeur(), img, 0, 0); // _taille -1 pour s'arretter au bon moment
}

//------------------------------------------------------------------------------
ImagePNG QuadTree::exporter() const{
	// reconstitution de la taille d'un coté
	unsigned coteImage = 1;
	for(int i = 0; i < _taille; i++)
		coteImage *= 2;

    ImagePNG img(coteImage, coteImage);
	exporter_rec(&_racine, coteImage, img, 0, 0);

    return img;
}

//------------------------------------------------------------------------------
void QuadTree::compressionDelta(unsigned delta){ // A FINIR
	assert(delta < 255);
	unsigned tailleI = 1;
	for(int i = 0; i < _taille; i++)
		tailleI *= 2;

	if(delta == 0)
		compressionSansPerte_rec(&_racine, tailleI);
	else
		compressionDelta_rec(&_racine, tailleI, delta);
}

//------------------------------------------------------------------------------
void QuadTree::compressionPhi(unsigned phi){ // A FINIR
	compressionPhi_rec(&_racine, phi);
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
	for(int i=0; i < 4; i++)
		if(ptr->fils[i] != nullptr)
			destructeur(ptr->fils[i]);

	for(int j=0; j < 4; j++)
		delete ptr->fils[j];
}

//------------------------------------------------------------------------------
void QuadTree::importer_rec(Noeud * ptr, unsigned taille, const ImagePNG & img, unsigned x, unsigned y){
	for(int i = 0; i < 4; i++){
		ptr->fils[i] = new Noeud;
		ptr->fils[i]->pere = ptr;
		for(int j = 0; j < 4; j++)
			ptr->fils[i]->fils[j] = nullptr;
	}

	if(taille > 2){
		importer_rec(ptr->fils[0], taille/2, img, x, y);	
		importer_rec(ptr->fils[1], taille/2, img, x, y + (taille/2));
		importer_rec(ptr->fils[2], taille/2, img, x + (taille/2), y);		
		importer_rec(ptr->fils[3], taille/2, img, x + (taille/2), y + (taille/2));
	} else {
		ptr->fils[0]->rvb = img.lirePixel(x,y);
		ptr->fils[1]->rvb = img.lirePixel(x,y+1);
		ptr->fils[2]->rvb = img.lirePixel(x+1,y);
		ptr->fils[3]->rvb = img.lirePixel(x+1,y+1);
	}

	vector<Couleur> vectTmp;
	for(int i = 0; i < 4; i++)
		vectTmp.push_back(ptr->fils[i]->rvb);

	// Pour la valeur du pere (moyenne des fils)
	ptr->rvb = moyenne(vectTmp);
}

//------------------------------------------------------------------------------
void QuadTree::exporter_rec(const Noeud* ptr,  unsigned taille, ImagePNG & img, unsigned x, unsigned y) const {
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
	if(taille > 2){
		compressionSansPerte_rec(ptr->fils[0], taille/2);	
		compressionSansPerte_rec(ptr->fils[1], taille/2);
		compressionSansPerte_rec(ptr->fils[2], taille/2);		
		compressionSansPerte_rec(ptr->fils[3], taille/2);
	}
	
	if(ptr->rvb.R == ptr->fils[0]->rvb.R && ptr->rvb.V == ptr->fils[0]->rvb.V && ptr->rvb.B == ptr->fils[0]->rvb.B)
		destructeur(ptr);
}

//------------------------------------------------------------------------------
void QuadTree::compressionDelta_rec(Noeud* ptr, unsigned taille, unsigned delta){
	if(taille > 2){
		compressionDelta_rec(ptr->fils[0], taille/2, delta);	
		compressionDelta_rec(ptr->fils[1], taille/2, delta);
		compressionDelta_rec(ptr->fils[2], taille/2, delta);		
		compressionDelta_rec(ptr->fils[3], taille/2, delta);
	}

	bool estFeuille = true;
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			if(ptr->fils[i]->fils[j] != nullptr)
				estFeuille = false;

	if(estFeuille){ //verifaction que tout les fils sont des feuilles	
		unsigned maxLumFils = 0;
		for(int i=0; i<4; i++)
			if(diff_lum(ptr->fils[i]->rvb, ptr->rvb) > maxLumFils)
				maxLumFils = diff_lum(ptr->fils[i]->rvb, ptr->rvb);

		cout << maxLumFils << " : Lum / delta : " << delta << endl;
		if(maxLumFils <= delta)
			for(int j=0; j<4; j++){
				destructeur(ptr->fils[j]);
				ptr->fils[j] = nullptr; // si mi dans le destructeur sa ne marche pas 
			}
	}
}

//------------------------------------------------------------------------------
void QuadTree::compressionPhi_rec(Noeud* ptr, unsigned phi){

}
