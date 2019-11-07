#include <stdio.h>
#include <sys/stat.h>
#include <err.h>
void print_stat(struct stat *file)
{
    printf("st_dev=%ld\n", file->st_dev);
    printf("st_ino=%ld\n", file->st_ino);
    printf("st_mode=%d\n", file->st_mode);
    printf("st_nlink=%ld\n", file->st_nlink);
    printf("st_uid=%d\n", file->st_uid);
    printf("st_gid=%d\n", file->st_gid);
    printf("st_rdev=%ld\n", file->st_rdev);
    printf("st_size=%ld\n", file->st_size);
    printf("st_atime=%ld\n", file->st_atime);
    printf("st_mtime=%ld\n", file->st_mtime);
    printf("st_ctime=%ld\n", file->st_ctime);
    printf("st_blksize=%ld\n", file->st_blksize);
    printf("st_blocks=%ld\n", file->st_blocks);
}

int main(int argc, char *argv[])
{
    struct stat file;
    if (argc > 1)
    {
        if (stat(argv[1], &file) == -1)
            return 1;
        print_stat(&file);
    }
    else
        return 1;
    return 0;
}
