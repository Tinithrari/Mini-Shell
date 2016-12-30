/********************************************//**
 * \file Redirection.h
 * \author Xavier Heugue
 * \see Commande.h
 ***********************************************/

/********************************************//**
 * \brief enum for flow
 ***********************************************/
enum flow
{
	IN,/**< Flux en entrée */
	OUT, /**< Flux en sortie */
	ERR, /**< Flux en erreur */
	ERROUT, /**< Flux mixé en sortie-erreur */
	APPOUT, /**< Flux append en sortie */
	APPERR, /**< Flux append en erreur */
	APPERROUT, /**< Flux append en sortie-erreur*/
	PIPE /**< Un tube */
};


 /********************************************//**
 * \brief Raccourci pour les flux
 ***********************************************/
typedef enum flow Flow;

 /********************************************//**
 * \brief Tubes de communication
 ***********************************************/
typedef int *tube;

/********************************************//**
 * \brief Rédirige le flux d'entrée de la commande
 *
 * \param c La commande à rediriger
 * \param p Le chemin du fichier
 * \return 1 si la commande a été redirigée, 0 sinon.
 *
 ***********************************************/
int setInCommande(Commande *c, string p);

/********************************************//**
 * \brief Redirige le flux de sortie de la commande
 *
 * \param c La commande à rediriger
 * \param p Le chemin du fichier
 * \return 1 si la commande a été redirigée, 0 sinon.
 *
 ***********************************************/
int setOutCommande(Commande *c, string p, int append);


/********************************************//**
 * \brief Redirige le flux des erreurs de la commande
 *
 * \param c La commande à rediriger
 * \param p Le chemin du fichier
 * \return 1 si la commande a été redirigée, 0 sinon
 *
 ***********************************************/
int setErrCommande(Commande *c, string p, int append);

/********************************************//**
 * \brief Créé un tube de communication entre deux processus
 *
 * \param c1 La commande dont on doit rediriger la sortie
 * \param c2 La commande dont on doit rediriger l'entrée
 * \return 1 si le pipe a bien été créé
 *
 ***********************************************/
int pipeCommande(Commande *c1, Commande *c2);

/********************************************//**
 * \brief Nettoie l'environnement d'exécution des commandes
 ***********************************************/
void cleanCommandeEnv(void);
