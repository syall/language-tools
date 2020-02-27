#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

// State
int LEX_START = 0;
int LEX_END = 0;
int VAR_LENGTH = 40;

// Tokens
Token *tokens = NULL;
Token *tail = NULL;

// Helper Methods
int alpha(char c);
int numer(char c);
int alphanumeric(char c);
int beginString(char c);
int endString(char c1, char c2);
int withinLength(int LEX_END, int LEX_START);
int isKeyword(char *word);
int isEqual(char c);
int isBang(char c);
int isLG(char c);
int isDeno(char c);
int isOperator(char c);
int isSep(char c);
int isHash(char c);
int isWS(char c);
int isLeft(char c);
int isRight(char c);

// Lexemes
int varOrKey(char *buffer);
int number(char *buffer);
int string(char *buffer);
int equalOrAssign(char *buffer);
int notEqualorNot(char *buffer);
int lesserOrGreaterWithEqual(char *buffer);
int denote(char *buffer);
int opWithEqualOrDouble(char *buffer);
int separator(char *buffer);
int left(char *buffer);
int right(char *buffer);
int lineComment(char *buffer);
int whitespace(char *buffer);

// Tokens
void printTokens();
Token *createToken(char *group, char *value);
Token *addToken(Token *add);

// Lexer
LexResult *lexer(char *buffer, int size)
{
    // Reset
    LEX_START = 0;
    LEX_END = 0;
    tokens = NULL;

    // Lexer Result
    LexResult *result = malloc(sizeof(LexResult));

    // Loop
    while (LEX_END < size)
    {
        // Current
        char c = buffer[LEX_START];
        // Lexemes
        if (alpha(c)) // Variable or Keyword
            result->state = varOrKey(buffer);
        else if (numer(c)) // Number
            result->state = number(buffer);
        else if (beginString(c)) // String
            result->state = string(buffer);
        else if (isEqual(c)) // "=" or "=="
            result->state = equalOrAssign(buffer);
        else if (isBang(c)) // "!" or "!="
            result->state = notEqualorNot(buffer);
        else if (isLG(c)) // "(>|<)="
            result->state = lesserOrGreaterWithEqual(buffer);
        else if (isDeno(c)) // Denotation
            result->state = denote(buffer);
        else if (isOperator(c)) // op or op=
            result->state = opWithEqualOrDouble(buffer);
        else if (isLeft(c)) // [, (, {
            result->state = left(buffer);
        else if (isRight(c)) // ], ), }
            result->state = right(buffer);
        else if (isSep(c)) // Separator
            result->state = separator(buffer);
        else if (isHash(c)) // Line Comment
            result->state = lineComment(buffer);
        else if (isWS(c)) // Whitespace
            result->state = whitespace(buffer);
        else // Error
            result->state = GENERAL_ERROR;

        // Check Result
        if (result->state != SUCCESS)
            return result;

        // Iterate LEX_END
        LEX_START = LEX_END;
    }

    // Set Tokens
    result->tokens = tokens;

    // Return Lexer Result
    return result;
}

// Implementation of Helper Methods
int alpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int numer(char c)
{
    return c >= '0' && c <= '9';
}

int alphanumeric(char c)
{
    return alpha(c) || numer(c);
}

int beginString(char c)
{
    return c == '"';
}

int endString(char c1, char c2)
{
    return c1 != '\\' && c2 == '"';
}

int withinLength(int LEX_END, int LEX_START)
{
    return LEX_END - LEX_START <= VAR_LENGTH;
}

int isKeyword(char *word)
{
    return (strcmp(word, "def") == 0 ||
            strcmp(word, "while") == 0 ||
            strcmp(word, "for") == 0 ||
            strcmp(word, "continue") == 0 ||
            strcmp(word, "break") == 0 ||
            strcmp(word, "return") == 0 ||
            strcmp(word, "if") == 0 ||
            strcmp(word, "else") == 0 ||
            strcmp(word, "string") == 0 ||
            strcmp(word, "number") == 0 ||
            strcmp(word, "bool") == 0 ||
            strcmp(word, "fn") == 0 ||
            strcmp(word, "flex") == 0 ||
            strcmp(word, "array") == 0 ||
            strcmp(word, "of") == 0 ||
            strcmp(word, "true") == 0 ||
            strcmp(word, "false") == 0);
}

int isEqual(char c)
{
    return c == '=';
}

int isBang(char c)
{
    return c == '!';
}

int isLG(char c)
{
    return c == '<' || c == '>';
}

int isDeno(char c)
{
    return c == ':';
}

int isOperator(char c)
{
    return c == '+' || c == '-' || c == '/' || c == '*' || c == '%';
}

int isSep(char c)
{
    return c == ',';
}

int isHash(char c)
{
    return c == '#';
}

int isWS(char c)
{
    return c <= ' ';
}

int isLeft(char c)
{
    return c == '[' || c == '(' || c == '{';
}

int isRight(char c)
{
    return c == ']' || c == ')' || c == '}';
}

// Implementation of Lexemes
int varOrKey(char *buffer)
{
    char *word = malloc(sizeof(char) * (VAR_LENGTH + 1));
    while (alphanumeric(buffer[LEX_END]) && withinLength(LEX_END, LEX_START))
    {
        word[LEX_END - LEX_START] = buffer[LEX_END];
        LEX_END++;
    }
    word[LEX_END - LEX_START] = '\0';

    if (!withinLength(LEX_END, LEX_START))
    {
        printf("Error with \"%c\" at %d\n", buffer[LEX_START], LEX_END);
        return LENGTH_ERROR;
    }

    Token *token = createToken(isKeyword(word) ? "keyword" : "variable", word);
    addToken(token);

    return SUCCESS;
}

int number(char *buffer)
{
    char *word = malloc(sizeof(char) * (VAR_LENGTH + 1));
    while (numer(buffer[LEX_END]))
    {
        word[LEX_END - LEX_START] = buffer[LEX_END];
        LEX_END++;
    }
    word[LEX_END - LEX_START] = '\0';

    if (buffer[LEX_END] != '.')
    {
        Token *token = createToken("number", word);
        addToken(token);
        return SUCCESS;
    }

    word[LEX_END - LEX_START] = '.';
    LEX_END++;
    while (numer(buffer[LEX_END]))
    {
        word[LEX_END - LEX_START] = buffer[LEX_END];
        LEX_END++;
    }
    word[LEX_END - LEX_START] = '\0';

    Token *token = createToken("number", word);
    addToken(token);

    return SUCCESS;
}

int string(char *buffer)
{
    char *word = malloc(sizeof(char) * (VAR_LENGTH + 1));
    LEX_END += 1;
    LEX_START += 1;
    while (!endString(buffer[LEX_END - 1], buffer[LEX_END]))
    {
        word[LEX_END - LEX_START] = buffer[LEX_END];
        LEX_END++;
    }
    word[LEX_END - LEX_START] = '\0';
    LEX_END += 2;

    Token *token = createToken("string", word);
    addToken(token);

    return SUCCESS;
}

int equalOrAssign(char *buffer)
{
    if (buffer[LEX_END + 1] == '=')
    {
        char *word = malloc(sizeof(char) * 3);
        word[0] = buffer[LEX_END];
        word[1] = buffer[LEX_END + 1];
        word[2] = '\0';
        Token *token = createToken("equals", word);
        addToken(token);
        LEX_END += 2;
    }
    else
    {
        char *word = malloc(sizeof(char) * 2);
        word[0] = buffer[LEX_END];
        word[1] = '\0';
        Token *token = createToken("assign", word);
        addToken(token);
        LEX_END += 1;
    }
    return SUCCESS;
}

int notEqualorNot(char *buffer)
{
    if (buffer[LEX_END + 1] == '=')
    {
        char *word = malloc(sizeof(char) * 3);
        word[0] = buffer[LEX_END];
        word[1] = buffer[LEX_END + 1];
        word[2] = '\0';
        Token *token = createToken("not-equals", word);
        addToken(token);
        LEX_END += 2;
    }
    else
    {
        char *word = malloc(sizeof(char) * 2);
        word[0] = buffer[LEX_END];
        word[1] = '\0';
        Token *token = createToken("not", word);
        addToken(token);
        LEX_END += 1;
    }
    return SUCCESS;
}

int lesserOrGreaterWithEqual(char *buffer)
{
    if (buffer[LEX_END + 1] == '=')
    {
        char *word = malloc(sizeof(char) * 3);
        word[0] = buffer[LEX_END];
        word[1] = buffer[LEX_END + 1];
        word[2] = '\0';
        Token *token = createToken("less/greater-equals", word);
        addToken(token);
        LEX_END += 2;
    }
    else
    {
        char *word = malloc(sizeof(char) * 2);
        word[0] = buffer[LEX_END];
        word[1] = '\0';
        Token *token = createToken("less/greater-than", word);
        addToken(token);
        LEX_END += 1;
    }
    return SUCCESS;
}

int denote(char *buffer)
{

    char *word = malloc(sizeof(char) * 2);
    word[0] = buffer[LEX_END];
    word[1] = '\0';
    Token *token = createToken("denotation", word);
    addToken(token);
    LEX_END += 1;
    return SUCCESS;
}

int opWithEqualOrDouble(char *buffer)
{
    if (buffer[LEX_END + 1] == buffer[LEX_END])
    {
        char *word = malloc(sizeof(char) * 3);
        word[0] = buffer[LEX_END];
        word[1] = buffer[LEX_END + 1];
        word[2] = '\0';
        Token *token = createToken("double-op", word);
        addToken(token);
        LEX_END += 2;
    }
    else if (buffer[LEX_END + 1] == '=')
    {
        char *word = malloc(sizeof(char) * 3);
        word[0] = buffer[LEX_END];
        word[1] = buffer[LEX_END + 1];
        word[2] = '\0';
        Token *token = createToken("op-equals", word);
        addToken(token);
        LEX_END += 2;
    }
    else
    {
        char *word = malloc(sizeof(char) * 2);
        word[0] = buffer[LEX_END];
        word[1] = '\0';
        Token *token = createToken("operator", word);
        addToken(token);
        LEX_END += 1;
    }
    return SUCCESS;
}

int left(char *buffer)
{
    char *word = malloc(sizeof(char) * 2);
    word[0] = buffer[LEX_END];
    word[1] = '\0';
    Token *token = createToken("left", word);
    addToken(token);
    LEX_END += 1;
    return SUCCESS;
}

int right(char *buffer)
{
    char *word = malloc(sizeof(char) * 2);
    word[0] = buffer[LEX_END];
    word[1] = '\0';
    Token *token = createToken("right", word);
    addToken(token);
    LEX_END += 1;
    return SUCCESS;
}

int lineComment(char *buffer)
{
    while (buffer[LEX_END] != '\n')
    {
        LEX_END += 1;
    }
    LEX_END += 1;
    return SUCCESS;
}

int separator(char *buffer)
{
    char *word = malloc(sizeof(char) * 2);
    word[0] = buffer[LEX_END];
    word[1] = '\0';
    Token *token = createToken("separator", word);
    addToken(token);
    LEX_END += 1;
    return SUCCESS;
}

int whitespace(char *buffer)
{
    LEX_END += 1;
    return SUCCESS;
}

// Tokens
void printTokens()
{
    Token *ptr = tokens;
    while (ptr != NULL)
    {
        printf("%s: %s\n", ptr->group, ptr->value);
        ptr = ptr->next;
    }
}

Token *createToken(char *group, char *value)
{
    Token *token = malloc(sizeof(Token));
    token->group = group;
    token->value = value;
    token->prev = NULL;
    token->next = NULL;
    return token;
}

Token *addToken(Token *add)
{
    if (tokens == NULL)
    {
        tokens = add;
        tail = tokens;
    }
    else
    {
        tail->next = add;
        add->prev = tail;
        tail = add;
    }
    return tail;
}
