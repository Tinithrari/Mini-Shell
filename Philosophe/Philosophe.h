/********************************************//**
 * \file Philosophe.h
 * \brief Définition d'un philosophe et de ses méthodes
 * \author Xavier Heugue
 * \see semaphore.h
 ***********************************************/

/********************************************//**
 * \brief État d'un philosophe
 ***********************************************/
enum etat
{
    PENSE = 0, /**< Le philosophe pense */
    FAIM = 1, /**< Le philosophe a faim */
    MANGE = 2 /**< Le philosophe mange */
};

/********************************************//**
 * \brief Structure d'un philosophe
 ***********************************************/
struct philosophe
{
    sem_t ustensiles; /**< Les ustensiles du philosophes */
    enum etat statut; /**< L'état du philosophe */
};

/********************************************//**
 * \brief Raccourci pour l'état du philosophe
 ***********************************************/
typedef enum etat Etat;
/********************************************//**
 * \brief Raccourci pour la structure philosophe
 ***********************************************/
typedef struct philosophe Philosophe;

/********************************************//**
 * \brief Initialise un philosophe
 *
 * \param Le philosophe a initialiser
 ***********************************************/
void initPhilosophe(Philosophe*);

/********************************************//**
 * \brief Le philosophe prend ses ustensiles (bloquant)
 *
 * \param Le philosophe qui doit prendre ses ustensiles
 ***********************************************/
void prendreUstensile(Philosophe*);

/********************************************//**
 * \brief Le philosophe réquisitionne les ustensiles
 *
 * \param Le philosophe voulant réquisitionner ses
 * ustensiles
 ***********************************************/
void requisitionneUstensile(Philosophe*);

/********************************************//**
 * \brief Le philosophe pense
 ***********************************************/
void pense();

/********************************************//**
 * \brief Le philosophe mange
 ***********************************************/
void mange();
