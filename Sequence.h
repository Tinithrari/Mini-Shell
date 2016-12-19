/********************************************//**
 * \brief Définitions pour les séquencements
 * \author Xavier Heugue
 * \file Sequence.h
 * \see Commande.h
 ***********************************************/


/********************************************//**
 * \brief opérateur logique
 ***********************************************/
enum logique
{
    NONE, /**< Aucun opérateur */
    AND, /**< ET logique */
    OR /**< OU logique */
};

/********************************************//**
 * \brief Décrit un élément séquence
 ***********************************************/
struct sequence
{
    Commande *c; /**< Commande de l'élément de séquence */
    enum logique l; /**< Opérateur logique mené à l'opérateur de séquence */
    struct sequence *suivant; /**< Pointeur vers l'élément suivant */
};

/********************************************//**
 * \brief Raccourci pour les opérateurs logiques
 ***********************************************/
typedef enum logique Logique;

/********************************************//**
 * \brief Raccourcis pour les séquences
 ***********************************************/
typedef struct sequence Sequence;

/********************************************//**
 * \brief Construit un nouvel élément de séquence
 *
 * \param c Commande associé à l'élément de séquence
 * \return
 *
 ***********************************************/
Sequence* newSequence (Commande *c);

/********************************************//**
 * \brief Relie deux séquences entre elles
 *
 * \param s1 Séquence de base
 * \param s2 Séquence à ajouter à la suite de la séquence de base
 * \param op Logique Condition pour laquelle exécuter s2
 * \return int 1 si le raccord à réussit, 0 en cas d'erreurs
 *
 ***********************************************/
int linkSequence(Sequence *s1, Sequence *s2, Logique op);

/********************************************//**
 * \brief Exécute une séquence
 *
 * \param s La séquence à exécuter
 * \return int 1 si l'exécution s'est bien passé, 0 en cas d'erreur d'exécution, -1 sinon
 *
 ***********************************************/
int executeSequence(Sequence *s);

/********************************************//**
 * \brief Supprime une séquence et ses éléments
 *
 * \param s La séquence à supprimer
 ***********************************************/
void deleteSequence(Sequence *s);
