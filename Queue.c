#include "Queue.h"
#include <stdlib.h>
#include <string.h>

Queue* createQueue()
{
	Queue *q;

	q = (Queue*) malloc(sizeof(Queue));

	// Si l'allocation ne réussis pas, return NULL
	if (q == NULL)
		return NULL;

	// Prépare les champs
	q->head = NULL;
	q->tail = NULL;

	return q;
}

int enqueue(Queue* queue, void *elt, size_t eltSize)
{
	QueueElt* qElt;

	// Vériification des paramètres
	if (queue == NULL || elt == NULL)
		return 0;

	qElt = (QueueElt*) malloc(sizeof(QueueElt));

	// Si l'allocation échoue, retourne 0
	if (qElt == NULL)
		return 0;

	qElt->elt = malloc(eltSize);

	// Retourne 0 si l'allocation echoue
	if (qElt->elt == NULL)
	{
		free(qElt);
		return 0;
	}

	// Copie de la structure
	memcpy(qElt->elt, elt, eltSize);
	qElt->next = NULL;

	// Remplit la file
	if (queue->head == NULL)
		queue->head = qElt;

	if (queue->tail != NULL)
		queue->tail->next = qElt;
	
	// Place l'élément en dernière position
	queue->tail = qElt;

	qElt->next = NULL;

	return 1;
}

QueueElt* dequeue(Queue* queue)
{
	QueueElt *elt;

	// Vérifie que la file ne soit pas vide ou que les arguments ne soient pas incorrect
	if (queue == NULL || queue->head == NULL)
		return NULL;

	elt = queue->head;

	queue->head = queue->head->next;
	
	// Si la tête de file est vide, on met la queue de la file à NULL
	if (queue->head == NULL)
		queue->tail = NULL;

	return elt;

}

int isQueueEmpty(Queue* q)
{
	if (q == NULL)
		return -1;
	return (q->head == NULL) && (q->tail == NULL);
}

int queueSize(Queue *q)
{
	QueueElt* elt;
	int cpt;

	if (q == NULL)
		return -1;

	// Itère dans la tete jusqu'à trouver l'élément NULL
	for (elt = q->head, cpt = 0; elt != NULL; elt = elt->next, cpt++);

	return cpt;
}

void freeQueueElt(QueueElt *elt)
{
	// Regarde si l'élément existe
	if (elt == NULL)
		return;

	free(elt->elt);
	free(elt);
}

void freeQueue(Queue *queue)
{
	QueueElt *elt;

	if (queue == NULL)
		return;
	
	for (elt = dequeue(queue); elt != NULL; elt = dequeue(queue))
		freeQueueElt(elt);

	free(queue);
}
