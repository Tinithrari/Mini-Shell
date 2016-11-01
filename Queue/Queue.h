/**
 * @file Queue.h
 * @brief Librairie implémentant la file chainée générique
 * @author Xavier Heugue
 * @version 1
 * @date 13 Octobre 2016
 */
#include <stddef.h>
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
	size_t eltSize; /*!< Taille d'un élément */
} Queue;

/**
 * @typedef struct queueElt QueueElt
 * @brief alias pour queueElt
 */

/**
 * @typedef struct queue Queue
 * @brief alias pour queue
 */

/**
 * @fn Queue* createQueue(size_t eltSize)
 * @brief Créer une file
 * @param eltSize Taille des éléments contenus dans la file
 * @return Retourne un pointeur sur la file créer, NULL si l'allocation mémoire échoue
 */
Queue* createQueue(size_t eltSize);

/**
 * @fn int enqueue(Queue *queue, void *elt)
 * @brief Enfile un élément à la file passé en paramètre
 * @param queue Pointeur sur la file où enfiler l'élément
 * @param elt pointeur sur l'élément à ajouter
 * @return 1 si l'élément a été enfilé, 0 si l'allocation échoue
 */
int enqueue(Queue *queue, void *elt);

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
 * @param queue La file à désallouer
 */
void freeQueue(Queue* queue);

/**
 * @fn void freeQueueElt(QueueElt *elt)
 * @brief Libère les ressources occupés à un élément de file
 * @param elt L'élément à libérer
 */
void freeQueueElt(QueueElt *elt);
