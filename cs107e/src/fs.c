/* 
 * File: fs.c
 * ----------
 *
 * Authored by ??
 *
 * Julie: not sure whether/how much this module has been tested
 *
 */

#include "fat.h"
#include "fs.h"
#include <stddef.h>



/* Tightly pack the MBR and FAT boot record structs so they match the
   on-disk layout. */
#pragma pack(push, 1)

struct dir_entry {
    // FILENAME.TXT:
    char filename[8]; // FILENAME
    char ext[3]; // TXT

    union {
        struct {
            unsigned read_only : 1;
            unsigned hidden : 1;
            unsigned system : 1;
            unsigned volume_label : 1;
            unsigned subdirectory : 1;
            unsigned archive : 1;
        
            unsigned _unused : 2;
        } fields;

        unsigned char val;
    } attrs;

    unsigned char case_nt; // Unused.

    unsigned char creation_time_ms;
    unsigned short creation_time;
    unsigned short creation_date;

    unsigned short last_access_date;

    unsigned short start_cluster_high;

    unsigned short time;
    unsigned short date;

    unsigned short start_cluster_low;

    unsigned size;
};
_Static_assert(sizeof(struct dir_entry) == 32,
               "Directory entry struct should be 32 bytes.");

#define ATTR_LFN 0x0F

/* Long file name (fake directory entry). */
struct lfn {
    unsigned char sequence;

    short name0[5];

    char attr; // 0x0F
    char entry_type; // 0x00
    char checksum;

    short name1[6];

    short _unused; // Always 0x0000?

    short name2[2];
};
_Static_assert(sizeof(struct lfn) == sizeof(struct dir_entry),
               "LFN should be same size as dir_entry.");

#pragma pack(pop)

struct node {
    int in_use;

    // Retain a way to get the referent dir entry for this node, so we
    // can update its size if needed.
    cluster_t dir_start_cluster;
    unsigned dir_entry_offset;

    unsigned size;

    cluster_t start_cluster;

    unsigned offset;
};

/* Implementation detail that dir_t and file_t are the same struct
   right now. */
struct file {
    struct node node;
};
_Static_assert(sizeof(struct file) == sizeof(struct node),
"File struct should be same as node struct.");
struct dir {
    struct node node;
};
_Static_assert(sizeof(struct dir) == sizeof(struct node),
"Dir struct should be same as node struct.");

#define MAX_OPEN_NODES 128

/* Allocation pool for node structs so that we can just return
   pointers and keep the type opaque to callers, and not leak memory
   as we would by using malloc w/o free. */
static struct node node_slots[MAX_OPEN_NODES];

static int lookup(dir_t *dir, char *path, struct node *node);

bool fs_init(void) {
    if (!fat_init()) return false;

    /* Initialize node descriptor slots. */
    for (int i = 0; i < MAX_OPEN_NODES; i++) {
        /* Technically, these are guaranteed to be 0 already,
           but... */
        node_slots[i].in_use = 0;
    }
    return true;
}

static unsigned node_read(struct node *node, void *buf, unsigned len) {
    unsigned bytes_read = fat_read(buf, node->start_cluster, node->offset, len);

    node->offset += bytes_read;
    return bytes_read;
}

static void lfn_entry_read(char *buf, struct lfn *lfn) {
    unsigned char sequence_num = lfn->sequence & 0b1111;
    
    // LFN entries are in reverse order in the directory.
    // So the first lfn_entry_read actually fills the tail of the filename.
    int pos = (sequence_num - 1) * 13; // # chars in an LFN entry.

    // Take the low-order byte of each 'UTF-16' character in the
    // filename.
    for (int i = 0; i < 5; i++) {
        buf[pos] = (char) lfn->name0[i];
        if (buf[pos] == '\0') return;
        pos++;
    }
    for (int i = 0; i < 6; i++) {
        buf[pos] = (char) lfn->name1[i];
        if (buf[pos] == '\0') return;
        pos++;
    }
    for (int i = 0; i < 2; i++) {
        buf[pos] = (char) lfn->name2[i];
        if (buf[pos] == '\0') return;
        pos++;
    }
}

static void filename_read(char *buf, const struct dir_entry *entry) {
    // Get 8.3\0 filename into one string.
    int i;
    for (i = 0; i < 8; i++) {
        if (entry->filename[i] == ' ') break;
        buf[i] = entry->filename[i];
    }

    buf[i] = '.';

    int j;
    for (j = 0; j < 3; j++) {
        if (entry->ext[j] == ' ') break;
        buf[i + 1 + j] = entry->ext[j];
    }

    if (j == 0) {
        buf[i] = '\0';
    } else {
        buf[i + 1 + j] = '\0';
    }
}

static int dir_read_name(dir_t *dir, struct dir_entry *out, char *namebuf) {
    int has_lfn = 0;
    while (1) {
        if (node_read(&dir->node, out, sizeof(struct dir_entry)) !=
                sizeof(struct dir_entry)) {
            return 1;
        }
        if (out->attrs.val != ATTR_LFN) {
            break;
        }
        has_lfn = 1;

        struct lfn *lfn = (struct lfn *) out;
        if (lfn->sequence == 0xE5) {
            // Deleted LFN.
            continue;
        }

        lfn_entry_read(namebuf, lfn);
    }

    if (out->filename[0] == 0x00) {
        return 1;
    } else if (out->filename[0] == 0xE5 || out->attrs.fields.volume_label) {
        return dir_read_name(dir, out, namebuf);
    }

    if (has_lfn == 0) {
        filename_read(namebuf, out);
    }

    return 0;
}

static char to_lower(char c) {
    if ('A' <= c && c <= 'Z') {
        return 'a' + (c - 'A');
    } else {
        return c;
    }
}

/* Finds the first component of path in dir. Returns how much of path
 * was consumed. */
static unsigned dir_find_and_open_path_component(dir_t *dir, char *path, struct node *out) {
    struct dir_entry entry;

    unsigned i;
    int found = 0;

    char namebuf[256];
    while (dir_read_name(dir, &entry, namebuf) == 0) {
        i = 0;

        while ((path[i] != '\0') && (path[i] != '/') &&
               (to_lower(path[i]) == to_lower(namebuf[i]))) {
            i++;
        }
        if ((to_lower(path[i]) != to_lower(namebuf[i])) &&
            !(path[i] == '/' || path[i] == '\0')) {
            continue;
        }

        found = 1;
        break;
    }

    if (!found) {
        return 0;
    }

    out->dir_start_cluster = dir->node.start_cluster;
    out->dir_entry_offset = dir->node.offset;

    out->size = entry.size;

    out->start_cluster =
        (entry.start_cluster_high << 16) |
        entry.start_cluster_low;

    out->offset = 0;

    return i;
}

static int lookup(dir_t *dir, char *path, struct node *out) {
    int i = 0;
    while (1) {
        int inc = dir_find_and_open_path_component(dir, &path[i], out);
        fs_dir_close(dir);

        if (inc == 0) {
            // No match was found.
            return 1;
        }
        i += inc;

        if (path[i] == '\0' || (path[i] == '/' && path[i + 1] == '\0')) {
            break;
        }

        // Still got more path components. Advance past the '/'.
        i += 1;

        dir = (dir_t *) out;
    }

    return 0;
}

static struct node *open(char *path) {
    struct node *node = NULL;
    for (int i = 0; i < MAX_OPEN_NODES; i++) {
        if (!node_slots[i].in_use) {
            node = &node_slots[i];
            break;
        }
    }

    // The slots are all in use. MAX_OPEN_NODES all open right now?
    if (node == NULL)
        return NULL;

    if (path[0] == '/' && path[1] == '\0') {
        // Base case: open the root directory.
        node->dir_start_cluster = 0;
        node->dir_entry_offset = 0;
        node->size = 0;
        node->start_cluster = fat_root_cluster();
        node->offset = 0;
        node->in_use = 1;
        return node;

    } else if (path[0] != '/') {
        // We don't support relative paths.
        return NULL;
    }

    if (lookup(fs_dir_open("/"), &path[1], node) != 0)
        return NULL;

    // Now that we're sure things are OK, actually mark the node as
    // allocated. (Not trying to be thread-safe -- if we were, someone
    // else could get this slot while we were doing the stuff above.)
    node->in_use = 1;
    return node;
}

dir_t *fs_dir_open(char *path) {
    return (dir_t *) open(path);
}

int fs_dir_read(dir_t *dir, dir_read_entry_t *out) {
    struct dir_entry entry;
    int result = dir_read_name(dir, &entry, out->name);
    if (result != 0) {
        return result;
    }

    out->subdirectory = entry.attrs.fields.subdirectory;
    out->size = entry.size;

    return 0;
}

unsigned fs_file_read(file_t *file, void *buf, unsigned len) {
    if (len > file->node.size - file->node.offset)
        len = file->node.size - file->node.offset;
    return node_read(&file->node, buf, len);
}

int fs_dir_close(dir_t *dir) {
    dir->node.in_use = 0;
    return 0;
}

file_t *fs_file_open(char *path) {
    return (file_t *) open(path);
}

int fs_file_close(file_t *file) {
    file->node.in_use = 0;
    return 0;
}


