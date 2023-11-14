#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define ReadBufferSize 1024
#define WriteBufferSize 1024
#define BufferFlush -1

/* for command chaining */
#define CommandNormal	0
#define CommandOr		1
#define CommandAnd		2
#define CommandChain	3

/* for ConvertNumber() */
#define ConvertLowercase	1
#define ConvertUnsigned	2

/* 1 if using system getline() */
#define UseGetline 0
#define UseStrtok 0

#define HistoryFile	".simple_shell_history"
#define HistoryMax	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@LineCount: the error count
 *@err_num: the error code for exit()s
 *@Line_CountFlag: if on count this line of input
 *@File_Name: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@Env_Chang: on if environ was changed
 *@Status: the return Status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@CMD_Buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int LineCount;
	int err_num;
	int Line_CountFlag;
	char *File_Name;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int Env_Chang;
	int Status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int CMD_Buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;


/* hsh.c */
int HshFunction(info_t *, char **);
int FindBuiltinFunction(info_t *);
void FindCmdFunction(info_t *);
void ForkCmdFunction(info_t *);

/* path.c */
int IsCmdFunction(info_t *, char *);
char *DupCharsFunction(char *, int, int);
char *FindPathFunction(info_t *, char *, char *);

/* loopHshFunction.c */
int loopHshFunction(char **);

/* err_string_functions.c */
void _eputs(char *);
int _eputchar(char);
int PutFdFunction(char c, int fd);
int PutsFdFunction(char *str, int fd);

/* string_functions.c */
int StrLenFunction(char *);
int StrCmpFunction(char *, char *);
char *StartsWithFunction(const char *, const char *);
char *StrCatFunction(char *, char *);

/* string_functions2.c */
char *StrCpyFunction(char *, char *);
char *StrDupFunction(const char *);
void PutsFunction(char *);
int _putchar(char);

/* string_functions3.c */
char *StrNCpyFunction(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* string_functions4.c */
char **StrToWordsFunction(char *, char *);
char **StrToWords2Function(char *, char);

/* memory_functions */
char *MemSetFunction(char *, char, unsigned int);
void FreeFunction(char **);
void *ReallocFunction(void *, unsigned int, unsigned int);

/* memory_functions2.c */
int BFreeFunction(void **);

/* more_functions.c */
int InteractiveFunction(info_t *);
int IsDelimiter(char, char *);
int IsAlpha(int);
int AtoiFunction(char *);

/* more_functions2.c */
int ErrorAtoi(char *);
void PrintError(info_t *, char *);
int PrintD(int, int);
char *ConvertNumber(long int, int, int);
void RemoveComments(char *);

/* builtin_emulators.c */
int MyExit(info_t *);
int MyCd(info_t *);
int MyHelp(info_t *);

/* builtin_emulators2.c */
int MyHistory(info_t *);
int MyAlias(info_t *);

/* getline.c module */
ssize_t GetInput(info_t *);
int GetLine(info_t *, char **, size_t *);
void SigintHandler(int);

/* info.c module */
void ClearInfo(info_t *);
void SetInfo(info_t *, char **);
void FreeInfo(info_t *, int);

/* env.c module */
char *GetEnv(info_t *, const char *);
int MyEnv(info_t *);
int MySetEnv(info_t *);
int MyUnSetEnv(info_t *);
int PopulateEnvList(info_t *);

/* env2.c module */
char **GetEnviron(info_t *);
int UnsetEnv(info_t *, char *);
int SetEnv(info_t *, char *, char *);

/* file_io_functions.c */
char *GetHistoryFile(info_t *info);
int WriteHistory(info_t *info);
int ReadHistory(info_t *info);
int BuildHistoryList(info_t *info, char *buf, int linecount);
int RenumberHistory(info_t *info);

/* liststr.c module */
list_t *AddNode(list_t **, const char *, int);
list_t *AddNodeEnd(list_t **, const char *, int);
size_t PrintListStr(const list_t *);
int DeleteNodeAtIndex(list_t **, unsigned int);
void FreeList(list_t **);

/* liststr2.c module */
size_t ListLength(const list_t *);
char **ListToStrings(list_t *);
size_t PrintList(const list_t *);
list_t *NodeStartsWith(list_t *, char *, char);
ssize_t GetNodeIndex(list_t *, list_t *);

/* chain.c */
int IsChain(info_t *, char *, size_t *);
void CheckChain(info_t *, char *, size_t *, size_t, size_t);
int ReplaceAlias(info_t *);
int ReplaceVariables(info_t *);
int ReplaceString(char **, char *);

#endif
