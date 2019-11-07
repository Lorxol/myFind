#include "parser.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    struct options *opt = malloc(sizeof(struct options));
    if (makeOptList(argc, argv, opt) == -1)
        return -1;
    printOpt(opt);
    destroyOpt(opt);
}
