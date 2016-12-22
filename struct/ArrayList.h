/**
 * @struct ArrayList
 * @brief ArrayList is a dynamic and generic vector
 *
 * A dynamic and generic array which resize itself when space is needed
 */
typedef struct
{
	void *array; /*<! Array of element */
	size_t eltSize; /*!< Size of an element in the array */
	size_t arrSize; /*!< Size of the array */
	size_t nbElement; /*!< Number of element in the array */
} ArrayList;

/** An equal test function */
typedef int (*eqFunc) (void*,void*);

/**
 * @fn ArrayList* createArray(size_t eltSize)
 * @param eltSize size of an element in the array
 * @return An ArrayList, if NULL is returned, please check errno
 */
ArrayList* createArray(const size_t eltSize);

/**
 * @fn int addInArray(ArrayList *a, void *elt)
 * @brief Add an element into the specified array list
 * @param a The array list where you want to add the element
 * @param elt The element to add in the array list
 * @return if 0 is returned, the allocation has fail and errno has been set, 1 if the add succeed
 */
int addInArray(ArrayList *a, const void *elt);

/**
 * @fn removeFromArray(ArrayList *a, void *elt, cmpFunc fct)
 * @brief Remove occurences of a specific element from the array
 * @param a The array list where there is an element to remove
 * @param elt The element to remove from the array
 * @param fct A comparison function to know what is the element to remove
 * @return if 0 is returned, please check errno, otherwise, the remove has succeeded and the number of remove element is returned
 */
int removeFromArray(ArrayList *a, void *elt, eqFunc fct);

/**
 * @fn removeFromArrayAt(ArrayList *a, unsigned int index)
 * @brief Remove the element at a specified position
 * @param a The array where you want to remove an element
 * @param index The position of the element in the array
 * @return A pointer on the removed element
 */
int removeFromArrayAt(ArrayList *a, const unsigned int index);

/**
 * @fn deleteArrayList(ArrayList *a)
 * @brief Remove an arraylist and all its component
 * @param a The array list you want to delete
 */
void deleteArray(ArrayList* a);
