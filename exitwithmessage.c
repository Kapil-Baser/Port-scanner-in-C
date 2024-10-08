#include "practical.h"

void exitWithUserMessage(const char *msg, const char *detail)
{
    fputs(msg, stderr);
    fputs(": ", stderr);
    fputs(detail, stderr);
    fputc('\n', stderr);
    exit(1);
}

void exitWithSystemMessage(const char *msg)
{
    perror(msg);
    exit(1);
}