/********************************************//**
 * \brief Pointeur sur fonction de hachage
 ***********************************************/
typedef int (*hachage)(void *elt);

/********************************************//**
 * \brief Pointeur sur fonction de comparaison
 ***********************************************/
typedef int (*compare)(void *elt1, void *elt2);

/********************************************//**
 * \brief Une association clé-valeur chainable
 ***********************************************/
struct pair
{
    void *key;
    void *elt;
};

/********************************************//**
 * \brief Une table de hachage
 ***********************************************/
struct hashmap
{
    size_t keySize;
    size_t eltSize;
    size_t mapSize;
    LinkedList* map;
    hachage hash;
    compare cmp;
};

/********************************************//**
 * \brief Raccourci pour association clé-valeur
 ***********************************************/
typedef struct pair Pair;

/********************************************//**
 * \brief Raccourci pour la table de hachage
 ***********************************************/
typedef struct hashmap Hashmap;

/********************************************//**
 * \brief Création d'une nouvelle table de hachage
 *
 * \param keySize Taille de la clé
 * \param eltSize Taille d'un élément
 * \param hash Fonction de hachage pour la clé
 * \param mapSize Taille de la table de hachage
 * \param cmp Fonction de comparaison des clés
 * \return Un pointeur sur une table de hachage
 ***********************************************/
Hashmap newHashmap(size_t keySize, size_t eltSize, size_t mapSize, hachage hash, compare cmp);

/********************************************//**
 * \brief Ajout d'un élément dans la table de hachage
 *
 * \param h Pointeur sur une table de hachage
 * \param key La clé de l'élément à ajouter
 * \param elt L'élément à ajouter
 * \return 1 si l'insertion a réussi, 0 sinon
 *
 ***********************************************/
int addEltHashmap(Hashmap *h, void *key, void *elt);

/********************************************//**
 * \brief Met à jour un élément de la table de hachage
 * s'il n'existe pas, il est ajouté
 *
 * \param h Un pointeur sur la table de hachage
 * \param key La clé de l'élément à ajouter
 * \param newValue Nouvelle valeur de la clé
 * \return 1 si l'insertion a réussi, 0 sinon
 ***********************************************/
int updateEltHashmap(Hashmap *h, void *key, void *newValue);

/********************************************//**
 * \brief Récupère un élément de la table de hachage
 *
 * \param h Un pointeur sur la table de hachage
 * \param key la clé de la valeur à récupérer
 * \return L'élément associé à la clé, ou NULL si non existant
 ***********************************************/
void* getEltHashmap(Hashmap *h, void *key);

/********************************************//**
 * \brief Retire un élément de la table de hachage
 *
 * \param h Un pointeur sur la table de hachage
 * \param key La clé de l'élément à récupérer
 * \return l'élément retiré de la table de hachage
 ***********************************************/
void* removeEltHashmap(Hashmap *h, void *key);

/********************************************//**
 * \brief Supprime la table de hachage et tous ses
 * éléments
 *
 * \param h Un pointeur sur la table de hachage
 ***********************************************/
void deleteHashmap(Hashmap *h);
