#define _GNU_SOURCE
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include "myfind.h"

//Makes the full name of the current file we explore
void makeNextPath(char *path, char *name, char *buf)
{
    int len = 0;
    for(; path[len] != '\0'; len++)
        continue;
    //Optimization possible by replacing i with len and deleting loop above
    for(int i = 0; path[i] != '\0'; i++)
        buf[i] = path[i];

    if (path[len - 1] != '/')
    {
        buf[len] = '/';
        len++;
    }
    for(int i = 0; name[i] != '\0'; i++)
        buf[len + i] = name[i];
}

//Test if file matches the expressions described in the chained list opt (not an AST)
int test(struct dirent *file, char *name, struct options *opt)
{

    if (opt != NULL)
    {
        if (strcmp(opt->opt, "-name") == 0)
        {
            if (file == NULL)
                return 0;
            //Check if there is an OR operator after
            if (opt->next != NULL && strcmp(opt->next->opt, "-o") == 0)
            {
                if (fnmatch(opt->arg, file->d_name, 0) == 0)
                    return 1;
                return test(file, name, opt->next->next);
            }
            //If the arg and the current file name match (default AND operator)
            if (fnmatch(opt->arg, file->d_name, 0) != 0)
                return 0;
        }
        else if (strcmp(opt->opt, "-type") == 0)
        {
            struct stat fileStat;
            if (stat(name, &fileStat) == -1)
            {
                printf("myfind: cannot stat");
                return -1;
            }

            if (opt->next != NULL && strcmp(opt->next->opt, "-o") == 0)
            {
                if (strcmp(opt->arg, "b") == 0 && S_ISBLK(fileStat.st_mode) != 0)
                    return 1;
                else if (strcmp(opt->arg, "c") == 0 && S_ISCHR(fileStat.st_mode) != 0)
                    return 1;
                else if (strcmp(opt->arg, "d") == 0 && S_ISDIR(fileStat.st_mode) != 0)
                    return 1;
                else if (strcmp(opt->arg, "f") == 0 && S_ISREG(fileStat.st_mode) != 0)
                    return 1;
                else if (strcmp(opt->arg, "l") == 0 && S_ISLNK(fileStat.st_mode) != 0)
                    return 1;
                else if (strcmp(opt->arg, "p") == 0 && S_ISFIFO(fileStat.st_mode) != 0)
                    return 1;
                else if (strcmp(opt->arg, "s") == 0 && S_ISSOCK(fileStat.st_mode) != 0)
                    return 1;  
                return test(file, name, opt->next->next);
            }
            if (strcmp(opt->arg, "b") == 0 && S_ISBLK(fileStat.st_mode) == 0)
                return 0;
            else if (strcmp(opt->arg, "c") == 0 && S_ISCHR(fileStat.st_mode) == 0)
                return 0;
            else if (strcmp(opt->arg, "d") == 0 && S_ISDIR(fileStat.st_mode) == 0)
                return 0;
            else if (strcmp(opt->arg, "f") == 0 && S_ISREG(fileStat.st_mode) == 0)
                return 0;
            else if (strcmp(opt->arg, "l") == 0 && S_ISLNK(fileStat.st_mode) == 0)
                return 0;
            else if (strcmp(opt->arg, "p") == 0 && S_ISFIFO(fileStat.st_mode) == 0)
                return 0;
            else if (strcmp(opt->arg, "s") == 0 && S_ISSOCK(fileStat.st_mode) == 0)
                return 0;
        }
        else if (strcmp(opt->opt, "-newer") == 0)
        {
            struct stat fileStat;
            if (stat(name, &fileStat) == -1)
            {
                printf("myfind: cannot stat");
                return -1;
            }
            struct stat optStat;
            if (stat(opt->arg, &optStat) == -1)
            {
                printf("myfind: cannot stat");
                return -1;
            }

            if (file != NULL && (strcmp(file->d_name, opt->arg) == 0 || fileStat.st_mtim.tv_sec < optStat.st_mtim.tv_sec))
                return 0;
            else if (file == NULL && (strcmp(name, opt->arg) == 0 || fileStat.st_mtim.tv_sec < optStat.st_mtim.tv_sec))
                return 0;
            else if (fileStat.st_mtime == optStat.st_mtime)
            {
                if (fileStat.st_mtim.tv_nsec < optStat.st_mtim.tv_nsec)
                    return 0;
            }
        }
        return test(file, name, opt->next);
    }
    else
        return 1;
    return 0;
}

int typeTest(char *name, char *argType)
{
    struct stat fileStat;
    if (stat(name, &fileStat) == -1)
    {
        printf("myfind: cannot stat %s\n", name);
        return -1;
    }

    if (strcmp(argType, "b") == 0 && S_ISBLK(fileStat.st_mode) != 0)
        return 1;
    else if (strcmp(argType, "c") == 0 && S_ISCHR(fileStat.st_mode) != 0)
        return 1;
    else if (strcmp(argType, "d") == 0 && S_ISDIR(fileStat.st_mode) != 0)
        return 1;
    else if (strcmp(argType, "f") == 0 && S_ISREG(fileStat.st_mode) != 0)
        return 1;
    else if (strcmp(argType, "l") == 0 && S_ISLNK(fileStat.st_mode) != 0)
        return 1;
    else if (strcmp(argType, "p") == 0 && S_ISFIFO(fileStat.st_mode) != 0)
        return 1;
    else if (strcmp(argType, "s") == 0 && S_ISSOCK(fileStat.st_mode) != 0)
        return 1;

    return 0;
}

// I know why type does not work on directories. We have to transmit the FULL path to stat
// and we only transmit the NAME when we are on a directory
int test2(struct dirent *file, char *path, char *argv[], int argc)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-name") == 0)
        {
            if (i + 1 >= argc)
            {
                printf("myfind: missing argument to %s", argv[i]);
                return 2;
            }
            // If we are a directory, the dir name is in path
            if (file == NULL && fnmatch(argv[i + 1], path, 0) != 0)
                return 0;
            // Else if we are a file
            else if (fnmatch(argv[i + 1], file->d_name, 0) != 0)
                return 0;
        }
        else if (strcmp(argv[i], "-type") == 0)
        {
            if (i + 1 >= argc)
            {
                printf("myfind: missing argument to %s", argv[i]);
                return 2;
            }

            if (file == NULL && typeTest(path, argv[i + 1]) != 1)
                return 0;
            else if (typeTest(path, argv[i + 1]) != 1)
                return 0;
        }
    }

    return 1;
}

// Test if name contains one of the dir names in argv
int testIsInDir(char *argv[], int argc, char *name)
{
    if (argc <= 1 || argv[1][0] == '-')
        return 1;
    for (int i = 1; i < argc && argv[i][0] != '-'; i++)
    {
        if (strstr(name, argv[i]) != NULL)
            return 1;
    }

    return 0;
}

int find(char *path, char *cur_name, char *argv[], int argc)
{
    DIR *dir = opendir(path);
    struct dirent *file = NULL;
    if (dir == NULL)
    {
        printf("myfind: %s: No such file or directory\n", path);
        return 1;
    }

    if (testIsInDir(argv, argc, path) && test2(file, cur_name, argv, argc) == 1)
    {
        printf("%s\n", path);
    }

    while((file = readdir(dir)) != NULL)
    {
        if (strcmp(file->d_name, ".") != 0 && strcmp(file->d_name, "..") != 0)
        {
            char next[300] = { 0 };
            makeNextPath(path, file->d_name, next);
            //if file is a directory, make a recursive call of find with file
            if (file->d_type == DT_DIR)
            {
                find(next, file->d_name, argv, argc);
            }
            else
            {
                //Else we check if the file name match the expressions
                if (testIsInDir(argv, argc, next) && test2(file, next, argv, argc) == 1)
                {
                    printf("%s\n", next);
                }
            }
        }
    }
    if (closedir(dir) == -1)
        return 1;
    return 0;
}
