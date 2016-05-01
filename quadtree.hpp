#ifndef __QUADTREE_HPP__
#define __QUADTREE_HPP__
/**
 * @file quadtree.hpp
 * @author 
 * @since 
 * @brief Fichier définissant une classe QuadTree pour représenter, 
 * et compresser, des images bitmap
 * 
 * À COMPLÉTER
**/

#include <queue>
#include <vector>
#include "imagepng.hpp"

/**
 * @brief Classe QuadTree qui représente une image bitmap sous forme arborescente
 * 
 * À COMPLÉTER
**/
class QuadTree
{
    public:
        /** --------------------------------------------------------------------
         * @brief Constructeur d'un quadtree vide
         * 
         * @b Complexité À COMPLÉTER
        **/
        QuadTree();

        /** --------------------------------------------------------------------
         * @brief Destructeur d'un quadtree
         * 
         * @b Complexité À COMPLÉTER
        **/
        ~QuadTree();

        /** --------------------------------------------------------------------
         * @brief Affichage textuel préfixe indenté du Quadtree
         * @attention À fin de débogage
         * 
         * @b Complexité À COMPLÉTER
        **/
        void afficher() const;

        /** --------------------------------------------------------------------
         * @brief Encodage d'une image dans le QuadTree
         * @param img L'image à représenter
         * @pre L'image est de taille 2^n * 2^n, avec n>0
         * 
         * @b Complexité À COMPLÉTER
        **/
        void importer(const ImagePNG & img);

        /** --------------------------------------------------------------------
         * @brief Exportation d'une image depuis le QuadTree
         * @return l'image bitmap correspondant au contenu du QuadTree
         * 
         * @b Complexité À COMPLÉTER
        **/
        ImagePNG exporter() const;

        /** --------------------------------------------------------------------
         * @brief Compression du QuadTree
         * @param delta le seuil maximal de différence de luminance (cf. sujet).
         * La valeur par défaut, delta=0, provoque une compression sans dégradation.
         * @pre delta<255
         * 
         * @b Complexité À COMPLÉTER
        **/
        void compressionDelta(unsigned delta);

        /** --------------------------------------------------------------------
         * @brief Compression du QuadTree
         * @param phi le nombre maximal de feuilles après compression (cf. sujet).
         * @pre phi>0 car on ne peut supprimer la racine
         * 
         * @b Complexité À COMPLÉTER
        **/
        void compressionPhi(unsigned phi);


    private:
    //////////////////////////////////////////////////////////////////////////////////////
    // Attributs et types internes représentant la structure arborescente
    //////////////////////////////////////////////////////////////////////////////////////

        // Taille de l'image représentée
        unsigned _taille;

        // Type des noeuds de l'arbre (structure pere-fils)
        struct Noeud
        {
            Noeud* pere; // nullptr si racine
            Noeud* fils[4]; // nullptr si feuille
            Couleur rvb; // du pixel si feuille, moyenne des fils sinon
        };
        
        // Noeud racine
        Noeud _racine;
		  
    //////////////////////////////////////////////////////////////////////////////////////
    // Fonctionnalités internes simplifiant l'implémentation des méthodes
    //////////////////////////////////////////////////////////////////////////////////////

        // Type interne représentant 1 bit (valeur 0 ou 1)
        typedef bool bit;

        // Fonction statique retournant la valeur du k-ième bit d'un entier positif n
        static bit kiemeBit(unsigned n, unsigned k);

        // Fonction statique réalisant l'affichage récursivement depuis le noeud *n
        static void afficher_rec(const Noeud * n, std::string tabs="");

        // Ajout personnel :
        /** --------------------------------------------------------------------
         * @brief Suppression du QuadTree (Utilisation lors de l'appelle du destructeur)
         * @param Pointeur vers Noeud pour savoir ou on est dans l'arbre
         * 
         * @b Complexité À COMPLÉTER
        **/
		void destructeur(Noeud * ptr);

        /** --------------------------------------------------------------------
         * @brief Creation du QuadTree (Utilisation lors de l'importation de l'image)
         * @param Pointeur vers Noeud pour savoir ou on est dans l'arbre, taille est la largeur de l'image, image est l'image qui est importé, x et y sont les positions pour placer les pixels au bon endroit dans l'arbre.
         * @pre image format 2^n * 2^n
         * 
         * @b Complexité À COMPLÉTER
        **/
		void importer_rec(Noeud * ptr, unsigned taille, const ImagePNG & img, unsigned x, unsigned y);
		
		/** --------------------------------------------------------------------
         * @brief Creation d'une image grace au QuadTree (Utilisation lors de l'exportation de l'image)
         * @param Pointeur vers Noeud pour savoir ou on est dans l'arbre, taille est la largeur de l'image, tabCpt est tableau contenant les zonnes par raport à la position dans l'arbre (position 0 = racine de l'arbre, position 1 un des fils de la racine, ... etc) et image est l'image qui est importé.
         * @pre image format 2^n * 2^n
         * 
         * @b Complexité À COMPLÉTER
        **/
		void exporter_rec(const Noeud* ptr, unsigned taille, ImagePNG & img, unsigned x, unsigned y) const;

		/** --------------------------------------------------------------------
         * @brief Compression d'une image sans perte (Utilisation lors de la compressionDelta de l'image)
         * @param ptr est le pointeur permettant de se mouvoir dans l'arbre
         * @pre delta == 0
         * 
         * @b Complexité À COMPLÉTER
        **/
		void compressionSansPerte_rec(Noeud* ptr, unsigned taille);

		/** --------------------------------------------------------------------
         * @brief Compression d'une image avec un delta (Utilisation lors de la compressionDelta de l'image)
         * @param ptr est le pointeur permettant de se mouvoir dans l'arbre, taille est la largeur de l'image et delta est le chiffre correspondant à la compression demandé
         * @pre delta < 255
         * 
         * @b Complexité À COMPLÉTER
        **/
		void compressionDelta_rec(Noeud* ptr, unsigned taille, unsigned delta);

		/** --------------------------------------------------------------------
         * @brief Compression d'une image avec un phi (Utilisation lors de la compressionPhi de l'image)
         * @param ptr est le pointeur permettant de se mouvoir dans l'arbre, taille est la largeur de l'image
         * @pre phi > 0
         * 
         * @b Complexité À COMPLÉTER
        **/
		void compressionPhi_rec(Noeud* ptr, unsigned phi, unsigned nbfeuille, unsigned taille);
};
#endif
