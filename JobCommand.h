
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
 * \brief Vérifie le fils mort et passe un message
 *
 * \param sig Code du signal (ici, SIGCHLD)
 ***********************************************/
void childDead(int sig);

/********************************************//**
 * \brief Passe un processus stoppé ou en arrière-plan en avant blanc
 *
 * \param pid Identifiant du processus ou -1
 ***********************************************/
void myfg(pid_t pid);

/********************************************//**
 * \brief passe un processus stoppé en arrière plan
 *
 * \param pid Identifiant du processus ou -1
 ***********************************************/
void mybg(pid_t pid);

/********************************************//**
 * \brief Affiche les jobs en cours
 ***********************************************/
void myjobs(void);
