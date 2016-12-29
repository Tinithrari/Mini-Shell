/********************************************//**
 * \brief Un Couple nom-valeur pour les variables
 ***********************************************/
struct coupleVariable
{
    char *name; /**< Nom de la variable */
    char *value; /**< Valeur de la variable */
};

/********************************************//**
 * \brief Raccourci pour le couple variable-nom
 ***********************************************/
typedef struct coupleVariable CoupleVariable;

/********************************************//**
 * \brief Découpe une chaine pour obtenir un nom
 * de variable et sa valeur
 *
 * \param str Chaine à découper
 * \param c Pointeur sur le couple à remplir
 * \return 1 si la recherche a été fructueuse, 0 sinon
 ***********************************************/
int cutVariable(char *str, CoupleVariable *c);
