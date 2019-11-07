#include "myfind.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    struct options *opt = malloc(sizeof(struct options));
    clearOpt(opt);
    if (argc > 1)
    {
        int err = 0;
        int i = 1;
        // For each directory given, we exec find on that directory
        for (; i < argc && argv[i][0] != '-'; i++)
        {
            err = find(argv[i], argv[i], argv, argc);
        }

        // If no directory is given, 'find -name foo' for example, we find on .
        if (i == 1)
            err = find(".", ".", argv, argc);

        return err;
    }
    else
    {
        if (find(".", ".", argv, argc) == 1)
            return 1;
    }
    return 0;
}
