//Affiche l'ensemble des variables d'environnements
void affiche_variables_environnement(void);

//Affiche la valeur associee a une chaine
char * getValeur(char * chaine);

//Initialise la memoire partagee
void init_shm(void);

//Supprime la memoire partagee
void delete_shm(void);

//Detache un segment de la memoire partagee
void detache_memoire(void);

//Determine si il y a des caracteres autre que '\0' sur un intervalle donné
int strlen_interv(int debut, int fin);

//Modifie la valeur associee a une variable
int modifier( char * nomVariable, char * valeurVariable);

//Inserer une chaine de caracteres dans une autre chaine de caracteres sans ecraser de contenu
int inserer(char * chaine);

//Teste si chaine est dans destination ou non
int estPresente (char * chaine);

//Retourne l'indice ou commence la chaine
int indice_debut (char * chaine);

//Supprime la chaine de destination
int supp_chaine (char * chaine);
