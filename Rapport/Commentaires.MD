#Commentaires pour la réalisation de la doc

Met ici tous les commentaires ou choses que tu voudrais que j'ajoute dans le rapport (ex : Implémentations des méthodes, complexités...)
J'utiliserai aussi ce fichier pour te transmettre mes éventuelles questions, jete un oeil sur le journal des commits de gitHub pour voir si je t'ai posé une question dessus.

##A supprimer avant de rendre le projet

###Notes de travail (HB) :
Listes des méthodes :
- QuadTree() :
	Complexité : θ(1)
	Implementation : Mais tous les pointeurs du noeud _racine à null et mais la taille à 0
	Commentaire suplementaire (difficulté rencontré) : Implementation d'initialisation

- ~QuadTree() :
	Complexité : Sa complexité dépend de la complexité de la methode destructeur(Noeud * ptr);
	Implementation : fait appel à la methode récursive destructeur(&_racine); qui va prendre en parametre un pointeur ver le noeud racine
	Commentaire suplementaire (difficulté rencontré) : aucune difficulté vu que tous se passe dans la methode destructeur(Noeud * ptr);

- afficher() :
	Complexité : 
		Pire cas : Sa complexité dépend de la complexité de la methode afficher_rec(Noeud * ptr);
		Meilleur cas : θ(1) (affiche une simple phrase)
	Implementation : soit l'algorithme ecrit une phrase soit il fait appel a une methode recursive afficher_rec(Noeud * ptr);
	Commentaire suplementaire (difficulté rencontré) : aucune difficulté vu que tous se passe dans la methode afficher_rec(Noeud * ptr);

- importer() :
	Complexité : θ(log(img.largeur()) + ?) -> ? est la complexité de la méthode récursive importer_rec(...);
	Implementation : calcule la taille de l'arbre via la largeur de l'image puis appel la méthode récursive importer_rec();
	Commentaire suplementaire (difficulté rencontré) : aucune difficulté vu que tous se passe dans la methode importer_rec();

- exporter() :
	Complexité : θ(taille + ?) -> ? est la complexité de la méthode récursive exporter_rec();
	Implementation : calcule la largeur de l'image puis utilise appel la méthode récursive exporter_rec(); 
	Commentaire suplementaire (difficulté rencontré) : aucune difficulté vu que tous se passe dans la methode exporter_rec();

- compressionDelta() :
	Complexité : θ(taille + ?) -> ? est la complexité de la méthode récursive compressionDelta_rec(); ou compressionSansPerte_rec(); (delta = 0)
	Implementation : Vériffie si le delta ne depasse pas 255 puis utilise une methode en fonction du delta 
	Commentaire suplementaire (difficulté rencontré) : aucune difficulté vu que tous se passe dans l'une des deux methodes compressionSansPerte_rec(); ou compressionDelta_rec();

- compressionPhi() :
	Complexité : θ(taille + ?) -> ? est la complexité de la méthode récursive compressionPhi_rec();
	Implementation : Verifie que le phi est superieur à 0 et on appel la méthode récursive compressionPhi_rec();
	Commentaire suplementaire (difficulté rencontré) : aucune difficulté vu que tous se passe dans la methode compressionPhi_rec();

- kiemeBit() :
	Complexité : θ(1)
	Implementation : Déja implementé quand on a commencé le projet

# Pas encore ecrit :
- destructeur() :
	Complexité :
	Implementation :
	Commentaire suplementaire (difficulté rencontré) :

- importer_rec() :
	Complexité :
	Implementation :
	Commentaire suplementaire (difficulté rencontré) :

- exporter_rec() :
	Complexité :
	Implementation :
	Commentaire suplementaire (difficulté rencontré) :

- compressionSansPerte_rec() :
	Complexité :
	Implementation :
	Commentaire suplementaire (difficulté rencontré) :

- compressionDelta_rec() :
	Complexité :
	Implementation :
	Commentaire suplementaire (difficulté rencontré) :

- compressionPhi_rec() :
	Complexité :
	Implementation :
	Commentaire suplementaire (difficulté rencontré) :
