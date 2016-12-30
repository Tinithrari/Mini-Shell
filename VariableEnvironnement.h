char * getValeur(char * chaine);

void init_shm(void);

void delete_shm(void);

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
