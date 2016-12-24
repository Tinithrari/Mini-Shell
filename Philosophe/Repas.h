/********************************************//**
 * \file Repas.h
 * \author Xavier Heugue
 * \brief Un repas de philosophe
 * \see Philosophe.h, pthread.h
 ***********************************************/

/********************************************//**
 * \brief Structure d'un repas
 ***********************************************/
struct repas
{
    Philosophe table[NB_PHILOSOPHES];
    pthread_mutex_t verrou;
};

/********************************************//**
 * \brief Un philosophe participe à un repas
 * il a donc une place
 ***********************************************/
struct place
{
    struct repas *r;
    int no;
};

/********************************************//**
 * \brief Raccourci pour la structure repas
 ***********************************************/
typedef struct repas Repas;

/********************************************//**
 * \brief Raccourci pour la structure place
 ***********************************************/
typedef struct place Place;

/********************************************//**
 * \brief Crée un repas
 *
 * \return Un pointeur sur un repas
 ***********************************************/
Repas *newRepas();

/********************************************//**
 * \brief Un philosophe vérifie la disponibilité
 * des couverts, il les réquisitionne si ils sont
 * disponibles
 *
 * \param La place du philosophe souhaitant
 * vérifier ses couverts
 ***********************************************/
void verifierCouvert(Place*);

/********************************************//**
 * \brief Le philosophe pose ses couverts, et
 * vérifie les couverts pour ses voisins
 *
 * \param La place du philosophe souhaitant poser
 * ses couverts
 ***********************************************/
void poserCouvert(Place*);

/********************************************//**
 * \brief Fait participer un philosophe à un repas
 *
 * \param La place du philosophe participant à un
 * repas
 ***********************************************/
void* participer(void*);

/********************************************//**
 * \brief Supprime un repas
 *
 * \param Le repas à supprimer
 ***********************************************/
void deleteRepas(Repas*);
