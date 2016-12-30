//Determine si il y a des caracteres autre que '\0' sur un intervalle donné
int strlen_interv(const char *s, int debut, int fin);

//Modifie la valeur associee a une variable
int modifier(char * destination, char * nomVariable, char * valeurVariable);

//Inserer une chaine de caracteres dans une autre chaine de caracteres sans ecraser de contenu
int inserer(char * destination, const char * chaine);

//Teste si chaine est dans destination ou non
int estPresente (char * destination, const char * chaine);

//Retourne l'indice ou commence la chaine
int indice_debut (const char * destination, char * chaine);

//Supprime la chaine de destination
int supp_chaine (char * destination, const char * chaine);
