/**
 * @struct Command
 *
 * Data structure which represents a command
 */
typedef struct
{
	char *app; /*!< Application name */
	int nbArgument; /*!< Number of argument */
	char **args; /*!< Array of arguments */
	int in; /*!< File descriptor for the stdin */
	int out; /*!< File descriptor for the stdout */
	int err; /*!< File descriptor for the stderr*/
} Command;

/**
 * @fn Command* createCommand()
 * @brief create an empty command
 * @return a pointer on the empty command
 *
 * Create an empty command with the default standard I/O value
 */
Command* createCommand();

/**
 * @fn int setApp(Command *c, char *name)
 * @brief set the command content
 * @param c The command to fill
 * @param name The app name to put in the command
 * @return 1 if the allocation succeed, 0 otherwise
 *
 * Set the application of the command and fill the first argument
 */
int setApp(Command *c, char* name);

/**
 * @fn int addArgument(Command *c, char* arg)
 * @brief Add an argument to the command
 * @param c The command where you want to add the argument
 * @param arg The argument to add to the command
 * @return 1 if the allocation succeed, 0 otherwise
 *
 * Add an argument to the command and decale the NULL terminator
 */
int addArgument(Command *c, char *arg);

/**
 * @fn freeCommandContent(Command *c)
 * @brief free the content of the command structure
 * @param c the command that you want to free the content
 *
 * Free the resources allocated inside the Command structure
 */
void freeCommandContent(Command *c);

/**
 * @fn freeCommand(Command *c)
 * @brief Free the command structure
 * @param c the command to free
 * 
 * Free the command and its content
 */
void freeCommand(Command *c);
