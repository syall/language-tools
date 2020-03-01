#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

Token *ptr = NULL;
ParseResult *result = NULL;

ParseResult *createParseResult(Tree *root)
{
    ParseResult *cur = malloc(sizeof(ParseResult));
    cur->state = SUCCESS;
    cur->tree = root;
    return cur;
}

Tree *createTree()
{
    Tree *cur = malloc(sizeof(Tree));
    cur->group = NULL;
    cur->name = NULL;
    cur->value = NULL;
    cur->type = NULL;
    cur->list = NULL;
    cur->prev = NULL;
    cur->next = NULL;
    return cur;
}

void addToList(ParseResult *res, Tree *tree)
{
    if (res->tree->list == NULL)
    {
        res->tree->list = tree;
        return;
    }
    res->tree->list->next = tree;
    tree->prev = res->tree->list->next;
    res->tree->list->next = tree;
}

void printList(Tree *root, int level)
{
    if (root->group != NULL)
    {
        for (int i = 0; i < level; i++)
            printf("  ");
        printf("group: %s\n", root->group);
    }
    if (root->name != NULL)
    {
        for (int i = 0; i < level; i++)
            printf("  ");
        printf("name: %s\n", root->name);
    }
    if (root->type != NULL)
    {
        for (int i = 0; i < level; i++)
            printf("  ");
        printf("type: %s\n", root->type);
    }
    if (root->value != NULL)
        printList(root->value, level + 1);
    Tree *list = root->list;
    while (list != NULL)
    {
        printList(list, level + 1);
        list = list->next;
    }
}

State declaration(ParseResult *root)
{
    ParseResult *res = createParseResult(createTree());
    if (strcmp("def", ptr->value) == 0)
    {
        res->tree->group = "declaration";
        ptr = ptr->next;
    }
    else
        return MATCH_ERROR;
    if (strcmp("variable", ptr->group) == 0)
    {
        res->tree->name = ptr->value;
        ptr = ptr->next;
    }
    else
        return MATCH_ERROR;
    if (strcmp(":", ptr->value) == 0)
    {
        ptr = ptr->next;
    }
    else
        return MATCH_ERROR;
    if (strcmp("keyword", ptr->group) == 0)
    {
        res->tree->type = ptr->value;
        ptr = ptr->next;
    }
    else
        return MATCH_ERROR;
    if (strcmp("=", ptr->value) == 0)
    {
        ptr = ptr->next;
    }
    else
        return MATCH_ERROR;

    addToList(root, res->tree);

    return SUCCESS;
}

// Parser
ParseResult *parser(Token *tokens)
{
    // Parser Result
    result = createParseResult(createTree());
    result->tree->group = "program";

    // Loop
    ptr = tokens;
    while (ptr != NULL)
    {
        // Rules
        char *value = ptr->value;
        if (strcmp("def", value) == 0)
            result->state = declaration(result);
        else
            result->state = GENERAL_ERROR;

        // Check State
        if (result->state != SUCCESS)
            return result;
        else
            printList(result->tree, 0);
    }

    // Return Parser Result
    return result;
}
