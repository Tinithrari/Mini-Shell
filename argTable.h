typedef char** argTable;

/**
 * @fn argTable cutString(char* string)
 * @brief Create a table which contain each element of the string and which is terminated by NULL
 * @param string The string to cut into a table
 * @return a pointer on the string table
 */
argTable cutString(char* string);

/**
 * @fn void freeArgTable(argTable tab)
 * @brief Free a table created with cutString
 * @param tab The table to free
 */
void freeArgTable(argTable tab);
