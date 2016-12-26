//Fonction de verification de la saisie afin d'intialiser les flags aux valeurs correspondantes
void initflags(int nbparam, char *lesparams[]);

//Fonction qui va stocker les fichiers en parametre sous forme de chaine de caracteres dans la Queue
Queue* stockageFichiers(int nbElt, char *nosFichiers[]);

//Ajoute tout les fichiers de argv sauf les options
Queue* deleteOption (int nbElt, char *tab[]);

//Verification des parametres (Gere que les repertoires pour le moment)
int verif_fichiers(int nbParam, char * chaine[]);

//Affichage des droits
void affiche_droits2(struct stat st, char buffer[]);

//Affichage du nom du fichier
void affiche_nom2(struct stat st, char buffer[]);

//Affichage des donnees
void affiche_donnees2(struct stat st, char buffer[]);

//Parcours le contenu de de la queue
void parcours_queue(Queue *q, int nb);

//Parcours le contenu du repertoire
void parcours_rep(Queue *q, DIR *d, char * chaine);

//Affichage des droits
void affiche_droits(Queue *q, struct stat st, struct dirent *elt, char buffer[]);

//Affichage de la date
void affiche_date(struct tm *mtime);

//Affichage du nom du fichier
void affiche_nom(struct stat st, char buffer[], struct dirent *elt);

//Affichage des donnees
void affiche_donnees(Queue *q, struct stat st, char buffer[], struct dirent *elt);
