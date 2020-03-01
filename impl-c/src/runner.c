#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv)
{
    printf("==============================================\n");

    // Check Argument Count
    if (argc != 2)
        return ARGUMENT_ERROR;

    // Stat
    struct stat stats;
    if (stat(argv[1], &stats) != 0)
        return FILE_ERROR;
    int size = (int)stats.st_size;

    // Open File Descriptor
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        return FILE_ERROR;

    // Read File
    char buffer[size + 1];
    if (read(fd, &buffer, size) == -1)
        return FILE_ERROR;
    close(fd);

    // Lexer
    LexResult *lexResult = lexer(buffer, size);
    if (lexResult->state != SUCCESS)
        printf("Status: %d\n", lexResult->state);

    // Parser
    ParseResult *parseResult = parser(lexResult->tokens);
    if (parseResult->state != SUCCESS)
        printf("Status: %d\n", parseResult->state);

    printf("==============================================\n");

    // Success
    return lexResult->state;
}
