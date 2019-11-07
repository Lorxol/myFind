#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

void clearOpt(struct options *list)
{
    for (int i = 0; i < 8; i++)
        list->opt[i] = '\0';
    for (int i = 0; i < 64; i++)
        list->arg[i] = '\0';
}

int makeOptList(int argc, char *argv[], struct options *list)
{
    int i = 2;
    if (argv[1][0] != '-')
        sprintf(list->arg, "%s", argv[1]);
    else
    {
        sprintf(list->opt, "%s", argv[1]);
        sprintf(list->arg, "%s", argv[2]);
        i++;
    }
    for (; i < argc; i++)
    {
        struct options *nxt = malloc(sizeof(struct options));
        list->next = nxt;
        clearOpt(nxt);
        sprintf(nxt->opt, "%s", argv[i++]);
        if (i == argc)
        {
            printOpt(list);
            destroyOpt(list);
            printf("myfind: expected expression before/after OR or AND operator\n");
            return -1;
        }
        if (nxt->opt[1] != 'o' && nxt->opt[1] != 'a')
        {
            sprintf(nxt->arg, "%s", argv[i]);
            if (i + 1 < argc && (strcmp(argv[i + 1], "-name") == 0 || strcmp(argv[i + 1], "-type") == 0))
            {
                //Default operator is and
                struct options *nxt2 = malloc(sizeof(struct options));
                clearOpt(nxt2);
                list->next = nxt;
                sprintf(nxt2->opt, "-a");
                nxt->next = nxt2;
                list = nxt2;
            }
            else
                list = nxt;
        }
        else
        {
            nxt->arg[0] = '\0';
            list = nxt;
            i -= 1;
        }
    }
    list->next = NULL;
    return 0;
}

void printOpt(struct options *list)
{
    printf("OPT: %s\nARG: %s\n", list->opt, list->arg);
    if (list->next != NULL)
        printOpt(list->next);
}

void destroyOpt(struct options *list)
{
    if (list->next != NULL)
    {
        destroyOpt(list->next);
    }
    free(list);
}
