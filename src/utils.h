#ifndef UTILS
#define UTILS

// State
typedef enum
{
    SUCCESS = 0,
    GENERAL_ERROR = 1,
    ARGUMENT_ERROR = 2,
    FILE_ERROR = 3,
    LENGTH_ERROR = 4,
    MATCH_ERROR = 5
} State;

// Token
typedef struct token
{
    char *group;
    char *value;
    int pos[2];
    struct token *prev;
    struct token *next;
} Token;

// Lex Result
typedef struct lexResult
{
    Token *tokens;
    State state;
} LexResult;

// Token
typedef struct tree
{
    char *group;
    char *name;
    char *type;
    struct tree *value;
    struct tree *list;
    struct tree *prev;
    struct tree *next;
} Tree;

// Parse Result
typedef struct parseResult
{
    Tree *tree;
    State state;
} ParseResult;

#endif
