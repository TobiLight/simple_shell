#ifndef SHELLX_H
#define SHELLX_H

/**
 * File: shellx.h
 * Authors: Oluwatobiloba Light
 *          Princewill Chimdi Samuel
 * Description: Contains structs, typedefs, function 
 *              prototypes and global variables used
 *              throughout this project.
*/

#define SHELLX_END_OF_FILE -2
#define SHELLX_EXIT -3

extern char **environ;
char *name;
int history;

/**
 * struct sh_singly_list - struct type defining a linked list.
 * @dir: A directory path.
 * @next: A pointer to another struct sh_singly_list.
 */
typedef struct sh_singly_list
{
	char *dir;
	struct sh_singly_list *next;
} sh_singly_list_t;

/**
 * struct sh_builtin - struct type defining builtin commands.
 * @name: The name of the builtin command.
 * @func: A function pointer to the builtin command's function.
 */
typedef struct sh_builtin
{
	char *name;
	int (*func)(char **argv, char **head);
} sh_builtin_t;

/**
 * struct sh_alias - struct defining aliases.
 * @name: The name of the alias.
 * @val: The value of the alias.
 * @next: A pointer to another struct sh_alias.
 */
typedef struct sh_alias
{
	char *name;
	char *val;
	struct sh_alias *next;
} sh_alias_t;

/* Global aliases linked list */
sh_alias_t *aliases;

/* STRING FUNCTIONS */
char *shellx_strcat(char *, const char *);
char *shellx_strncat(char *, const char *, size_t);
char *shellx_strchr(char *, char);
char *shellx_strcpy(char *, const char *);
int shellx_strcmp(char *, char *);
int shellx_strncmp(const char *, const char *, size_t);
int shellx_strspn(char *, char *);
int shellx_strlen(const char *);
char **shellx_strtok(char *, char *);
char *shellx_strdup(const char *);
int shellx_token_len(char *, char *);
int shellx_count_tokens(char *, char *);

/* HELPERS */
ssize_t shellx_getline(char **, size_t *, FILE *);
int shellx_execute(char **, char **);
char *shellx_itoa(int);
sh_singly_list_t *shellx_get_path_dir(char *);
char *shellx_fill_path_dir(char *);
void *shellx_realloc(void *, unsigned int, unsigned int);
void *shellx_memcpy(void *, const void *, size_t);
void shellx_free_list(sh_singly_list_t *);
char *shellx_get_loc(char *);
void shellx_assign_lineptr(char **, size_t *, char *, size_t);

int shellx_proc_file_commands(char *, int *);
char *shellx_get_pid(void);
char *shellx_get_env_value(char *, int);

/* INPUT HANDLERS */
int shellx_handle_args(int *);
char *shellx_get_args(char *, int *);
void shellx_handle_line(char **, ssize_t);
void shellx_replace_variable(char **, int *);
int shellx_run_args(char **, char **, int *);
int shellx_call_args(char **, char **, int *);
int shellx_check_args(char **);
void shellx_free_args(char **, char **);
char **shellx_replace_aliases(char **);
ssize_t shellx_get_new_len(char *);
void shellx_logical_ops(char *, ssize_t *);

int (*shellx_get_builtin(char *))(char **, char **);
int shellx_exit(char **, char **);
int shellx_env(char **, char __attribute__((__unused__)) **);
int shellx_setenv(char **, char __attribute__((__unused__)) **);
int shellx_unsetenv(char **, char __attribute__((__unused__)) **);
int shellx_cd(char **, char __attribute__((__unused__)) **);
int shellx_alias(char **, char __attribute__((__unused__)) **);
int shellx_help(char **, char __attribute__((__unused__)) **);
void shellx_set_alias(char *, char *);
void shellx_print_alias(sh_alias_t *);

/* ERROR HANDLING */
int shellx_create_error(char **, int);
char *shellx_error_env(char **);
char *shellx_error_1(char **);
char *shellx_error_2_exit(char **);
char *shellx_error_2_cd(char **);
char *shellx_error_2_syntax(char **);
char *shellx_error_126(char **);
char *shellx_error_127(char **);

/* BUILTIN HANDLERS */
void shellx_help_setenv(void);
void shellx_help_cd(void);
void shellx_help_exit(void);
void shellx_help_env(void);
void shellx_help_help(void);
void shellx_help_history(void);
void shellx_help_alias(void);
void shellx_help_unsetenv(void);
void shellx_help_all(void);

/* LIST HANDLERS */
sh_alias_t *shellx_add_alias_end(sh_alias_t **, char *, char *);
sh_singly_list_t *shellx_add_node_end(sh_singly_list_t **, char *);
void shellx_free_alias_list(sh_alias_t *);
void shellx_free_list(sh_singly_list_t *);

/* BUILTIN HELPERS */
char **shellx_copyenv(void);
void shellx_free_env(void);
char **shellx_getenv(const char *);

void shellx_sig_handler(int);

#endif /* SHELLX_H */
