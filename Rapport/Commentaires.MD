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
	Complexité : θ(?) -> ? est la complexité de la methode récursive destructeur(Noeud * ptr);
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

- destructeur() :
	Complexité : θ(nombreDeNoeud)
	Implementation : Parcour l'ensemble des noeuds (récursivement) et delete les pointeurs de chaque noeud en commencant par les feuilles et en remontant l'arbre progressivement
	Commentaire suplementaire (difficulté rencontré) : la difficulté a été de penser au cas d'arret et au parcours de l'arbre

- importer_rec() :
	Complexité : θ(nombreDeNoeud)
	Implementation : Créé des Noeuds au fur et à mesure et insert dans ceci la couleur ou la moyenne de la couleur par rapport à l'image
	Commentaire suplementaire (difficulté rencontré) : la difficulté a été la création des noeuds et surtout de mettre les bon pixel au bon endroit dans l'arbre (On a également eu une difficulté suplémentaire pour la moyenne des pixels pour les noeuds non-feuille)

- exporter_rec() :
	Complexité : θ(nombreDeNoeud)
	Implementation : Parcour les Noeuds et ecrit les pixels grace au couleur des feuilles dans une image vierge ( vierge : a deja les bonne dimensions)  
	Commentaire suplementaire (difficulté rencontré) : la difficulté a été de faire correspondre des arbres à une image (même pour des arbre désiquilibré) 

- compressionSansPerte_rec() :
	Complexité : θ(nombreDeNoeud)
	Implementation : Parcour les Noeud à la recherche de Noeud et de leur fils qui serai pareil pour les suprimés
	Commentaire suplementaire (difficulté rencontré) : la difficulté a été de parcourir l'arbre et de supprimée les zonnes ou les couleurs était les mêmes (entre pere et fils)

- compressionDelta_rec() :
	Complexité : θ(nombreDeNoeud)
	Implementation : Parcour l'arbre à la recherche des feuilles qui sont inferieur ou égal à delta pour les suprimmées
	Commentaire suplementaire (difficulté rencontré) : la difficulté a été de parcourir l'arbre et ensuite de supprimé les zonnes qui était inferieur ou égale au delta donnée en paramètre

# Pas encore sur :
- compressionPhi_rec() : Pas encore sur il me reste à l'implementer
	Complexité : θ(nombreDeNoeud + TailleDuTableau) le tableau en question doit conserver les feuilles et leur luminescence
	Implementation : Parcour l'arbre à la recherche de feuille à suprimmée pour que le nombre de feuilles soit égal au phi
	Commentaire suplementaire (difficulté rencontré) : la difficulté a été le parcours de l'arbre, la suppression par rapport à phi et surtout de garder les bonnes zonnes à suprimé (demande beaucoup de mémoire)
