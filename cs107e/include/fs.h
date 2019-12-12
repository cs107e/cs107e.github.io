#ifndef FS_H
#define FS_H

#include <stdbool.h>

typedef struct {
    char name[256];

    int subdirectory;
    unsigned size;
} dir_read_entry_t;

bool fs_init(void);

typedef struct dir dir_t;

dir_t *fs_dir_open(char *path);
int fs_dir_read(dir_t *dir, dir_read_entry_t *out);
int fs_dir_close(dir_t *dir);

typedef struct file file_t;

file_t *fs_file_open(char *path);
unsigned fs_file_read(file_t *file, void *buf, unsigned len);
int fs_file_close(file_t *node);

#endif
