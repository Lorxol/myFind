
#ifndef PARSER
#define PARSER
struct options
{
    char opt[8];
    char arg[64];
    struct options *next;
};


int makeOptList(int argc, char *argv[], struct options *list);


void printOpt(struct options *list);

void clearOpt(struct options *list);

void destroyOpt(struct options *list);
#endif
