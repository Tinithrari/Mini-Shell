/**
 * @file Queue.h
 * @brief Librairie implémentant la file chainée générique
 * @author Xavier Heugue
 * @version 1
 * @date 13 Octobre 2016
 */

/**
 * @struct queueElt
 * @brief Element de la file
 * 
 * QueueElt est un élément de file chainée contenant un pointeur sur l'élément actuel,
 * un pointeur sur le QueueElt suivant et la taille de l'élément.
 */
typedef struct queueElt {
	void *elt; /*!< Pointeur sur l'élément courant */
	struct queueElt *next; /*!< Pointeur sur le prochain élément de la file */
	ssize_t eltSize; /*!< Taille de l'élément courant  */
} QueueElt;

/**
 * @struct queue
 * @brief File générique
 * 
 * Queue est une file générique ayant un pointeur sur son premier élément et sur son dernier élément.
 */
typedef struct queue{
	QueueElt *head; /*!< Pointeur sur le premier élément de la file */
	QueueElt *tail; /*!< Pointeur sur le dernier élément de la file */
} Queue;

/**
 * @typedef struct queueElt QueueElt
 * @brief alias pour struct queueElt
 */

/**
 * @typedef struct queue Queue
 * @brief alias pour struct queue
 */

/**
 * @fn Queue* createQueue
 * @brief Créer une file
 * @return Retourne un pointeur sur la file créer, NULL si l'allocation mémoire échoue
 */
Queue* createQueue();

/**
 * @fn int enqueue(Queue *queue, void *elt, ssize_t size)
 * @brief Enfile un élément à la file passé en paramètre
 * @param queue Pointeur sur la file où enfiler l'élément
 * @param elt pointeur sur l'élément à ajouter
 * @param size taille de l'élément à ajouter
 * @return 1 si l'élément a été enfilé, 0 si l'allocation échoue
 */
int enqueue(Queue *queue, void *elt, ssize_t size);

/**
 * @fn QueueElt* dequeue(Queue* queue)
 * @brief Défile un élément de la file passé en paramètre
 * @param queue La queue à défiler
 * @return Un pointeur sur le premier élément de la file, NULL si la file n'existe pas ou si la file est vide
 */
QueueElt* dequeue(Queue* queue);

/**
 * @fn int isQueueEmpty(Queue* queue)
 * @brief Indique si la file est vide
 * @param queue La file à analyser
 * @return 1 si la file est vide, 0 sinon
 */
int isQueueEmpty(Queue* queue);

/**
 * @fn int queueSize(Queue* queue)
 * @brief Donne la taille de la file
 * @param queue La file à analyser
 * @return la taille de la file, -1 si la file n'existe pas
 */
int queueSize(Queue* queue);

/**
 * @fn void freeQueue(Queue* queue)
 * @brief Libère la file de la mémoire et ses éléments
 * @param La file à désallouer
 */
 void freeQueue(Queue* queue);
