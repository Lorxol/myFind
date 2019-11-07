
#ifndef MYFIND
#define MYFIND
#include "parser.h"
#include <dirent.h>
int find(char *path, char *cur_name, char *argv[], int argc);
int test(struct dirent *file, char *name, struct options *opt);
#endif
