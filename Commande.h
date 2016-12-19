/********************************************//**
 * \file Commande.h
 * \author Xavier Heugue
 * \see Commande.h
 ***********************************************/

/********************************************//**
 * \brief Chaîne de caractères
 ***********************************************/
typedef char* string;

/********************************************//**
 * \brief Une commande et ses différents attributs
 ***********************************************/
struct commande
{
    string commande;/**< La commande a éxécuter */
    string *options;/**< Tableau d'option devant se terminer par NULL */
    unsigned int nOptions; /**< Nombre d'option contenu dans le tableau */
    int in;/**< File descritptor de l'entrée du programme */
    int out;/**< File descriptor de la sortie du programme */
    int err;/**< File descriptor de la sortie d'erreur du programme */
    int background;/**< La commande doit-elle tournée en arrière-plan */
    int *toClose;/**< Tableau de file descriptor à fermer */
    unsigned int nFd; /**< Nombre de file descriptor dans le tableau */
};

/********************************************//**
 * \brief Une commande sans le préfix struct
 ***********************************************/
typedef struct commande Commande;


/********************************************//**
 * \brief Construit une commande
 *
 * \param s La commande à exécuter
 * \return Un pointeur sur la commande si l'allocation réussit, NULL sinon
 *
 ***********************************************/
Commande* newCommande(string s);

/********************************************//**
 * \brief Ajout d'options dans une commande
 *
 * \param c La commande à laquelle on souhaite ajouter une option
 * \param p Une option
 * \return 1 si l'option a été ajouté, 0 sinon
 *
 ***********************************************/
int addOptionCommande(Commande *c, string p);

/********************************************//**
 * \brief Exécute une commande
 *
 * \param c La commande a exécuter
 * \return 1 si la commande a bien été exécutée, 0 sinon
 *
 ***********************************************/
int executeCommande(Commande *c);

/********************************************//**
 * \brief Supprime une commande et ressources associées
 *
 * \param c La commande à supprimer
 *
 ***********************************************/
void deleteCommande(Commande *c);
