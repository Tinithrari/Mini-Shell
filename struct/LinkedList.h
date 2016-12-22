/********************************************//**
 * \brief Nœud de liste chainée
 ***********************************************/
struct nodeLL
{
    void *value; /**< La valeur du nœud */
    struct nodeLL *next; /**< Pointeur vers l'élément suivant */
};

/********************************************//**
 * \brief Liste chainée
 ***********************************************/
struct linkedlist
{
    struct nodeLL *head; /**< Pointeur sur la tête */
    struct nodeLL *tail; /**< Pointeur sur la queue */
    size_t eltSize; /**< Taille des élément de la liste chainée */
    size_t nElement; /**< Nombre d'éléments dans la liste */
};

/********************************************//**
 * \brief Raccourci pour la liste chainée
 ***********************************************/
typedef struct linkedlist LinkedList;

/********************************************//**
 * \brief initialise une liste chainée
 *
 * \param ll Un pointeur sur liste chainée
 * \param eltSize La taille des éléments à stocker
 ***********************************************/
void initLL(LinkedList *ll, size_t eltSize);

/********************************************//**
 * \brief Créer une nouvelle liste chainée
 *
 * \param eltSize La taille des éléments à stocker
 * \return Un pointeur sur la nouvelle liste chainée
 * ou NULL en cas d'erreur
 ***********************************************/
LinkedList* newLL(size_t eltSize);

/********************************************//**
 * \brief Ajoute un élément dans une liste chainée
 *
 * \param ll Un pointeur sur une liste chainée
 * \param elt L'élément à enregistrer
 * \return int 1 si l'ajout à réussit, 0 sinon
 ***********************************************/
int addEltLL(LinkedList *ll, void *elt);

/********************************************//**
 * \brief Récupère un élément dans une liste chainée
 * \param ll Un pointeur sur liste chainée
 * \param index La position de l'élément à récupérer
 * \return Un pointeur sur un élément de la liste
 ***********************************************/
void* getEltLL(LinkedList *ll, unsigned int index);

/********************************************//**
 * \brief Supprime un élément de la liste chainée
 *
 * \param ll Un pointeur sur une liste chainée
 * \param index La position de l'élément à supprimer
 * \return L'élément supprimer de la liste chainée
 ***********************************************/
void* removeEltLL(LinkedList *ll, unsigned int index);

/********************************************//**
 * \brief Libère tous les éléments de la liste chainée
 *
 * \param ll Un pointeur sur une liste chainée
 ***********************************************/
void cleanLL(LinkedList *ll);

/********************************************//**
 * \brief Supprime une liste chainée et ses éléments
 *
 * \param ll Un pointeur sur une liste chainée
 ***********************************************/
void deleteLL(LinkedList *ll);
