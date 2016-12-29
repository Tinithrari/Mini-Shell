/********************************************//**
 * \brief État du processus
 ***********************************************/
enum etatProcessus
{
    RUNNING, /**< Processus en cours d'exécution */
    STOPPED /**< Processus stoppé */
};

/********************************************//**
 * \brief Structure du job
 ***********************************************/
struct job
{
    unsigned int noJob; /**< Numéro du job */
    pid_t pid; /**< Identifiant du processus */
    enum etatProcessus etat; /**< État du processus */
    Commande *commande; /**< La commande référencée pour le job */
};

/********************************************//**
 * \brief Raccourci pour le type etatProcessus
 ***********************************************/
typedef enum etatProcessus EtatProcessus;

/********************************************//**
 * \brief Raccourci pour la structure job
 ***********************************************/
typedef struct job Job;


/********************************************//**
 * \brief Initialisation du job
 *
 * \param j Un pointeur sur le job
 * \param pid Identifiant du processus
 * \param etat État du processus
 * \param commande Commande a rattacher au job
 ***********************************************/
void initJob(Job *j, pid_t pid, EtatProcessus etat, Commande *commande);

/********************************************//**
 * \brief Reset the job counter
 ***********************************************/
void resetCounter(void);
