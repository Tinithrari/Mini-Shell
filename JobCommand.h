
/********************************************//**
 * \brief Quitte le shell sans tuer la moindre commande en arrière plan
 ***********************************************/
void exitShell(void);

/********************************************//**
 * \brief Quitte le shell et tue toutes les commandes
 *
 * \param sig Code du signal reçu (ici, SIGINT)
 ***********************************************/
void interruption(int sig);

/********************************************//**
 * \brief Arrete le job en cours lorsq'un Ctrl+Z est utilisé
 *
 * \param sig Code du signal (ici, SIGSTOP)
 ***********************************************/
void stopJob(int sig);

/********************************************//**
 * \brief Affiche les jobs en cours
 ***********************************************/
void myjobs(void);
