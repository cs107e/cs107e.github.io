/* 
 * File: fat.c
 * -----------
 *
 * Authored by Omar
 *
 * Julie: not sure whether/how much this module has been tested
 *
 */

#include "sd.h"
#include "malloc.h"
#include "fat.h"


typedef unsigned sector_t;

#pragma pack(push, 1)

struct mbr {
    char boot_code[446];

    struct partition_entry {
        char boot_flag;

        char chs_begin[3]; // Old Cylinder/H/S scheme. Don't care.

        char type_code; // The important one. 0x0B or 0x0C for FAT32.

        char chs_end[3]; // Old CHS scheme. Don't care.

        // Sector where the partition starts. The other important one.
        sector_t lba_begin;

        unsigned sector_count;
    } partition_entries[4];

    short signature;
};
_Static_assert(sizeof(struct mbr) == SD_SECTOR_SIZE,
               "MBR struct should be sector-size.");

struct fat_boot_record {
    char jmp[3];
    char oem[8];

    // FAT uses the term 'sector', but I prefer 'sector'.
    // Except here, since I want to be able to follow docs.

    short bytes_per_sector;
    unsigned char sectors_per_cluster;

    unsigned short num_reserved_sectors;

    unsigned char num_fats;
    unsigned short num_dir_entries;

    unsigned short unused_num_sectors; // Should be 0 for a 4GB+ disk.

    char media_descriptor;

    short _unused1; // For FAT12/FAT16: # sectors per FAT.
    short _unused2; // Sectors per track.
    short _unused3; // # heads or sides on media.
    unsigned num_hidden_sectors; // ???

    unsigned num_sectors; // Actual # of sectors. Too big for a uint16.

    // Extended boot record (FAT32-specific):
    unsigned sectors_per_fat;
    unsigned short flags;
    unsigned short ver;
    cluster_t root_cluster;

    // Unused?
    unsigned short fsinfo_sector;
    unsigned short backup_boot_sector;
    char reserved[12]; // Should be 0.
    char drive_number;
    char nt_flags;

    char signature; // 0x28 or 0x29.

    unsigned volume_serial;
    char volume_label[11];
    char system_id[8]; // "FAT32   "

    char boot_code[420];
    short boot_signature; // 0xAA55 if bootable?
};
_Static_assert(sizeof(struct fat_boot_record) == SD_SECTOR_SIZE,
               "FAT boot record struct should be sector-size.");

#pragma pack(pop)

#define STRINGIFY(FIELD)                                          \
        char FIELD[sizeof(((struct fat_boot_record *)0)->FIELD) + 1];   \
        for (int i = 0; i < sizeof(((struct fat_boot_record *)0)->FIELD); i++) \
            FIELD[i] = boot_record.FIELD[i];                            \
        FIELD[sizeof(((struct fat_boot_record *)0)->FIELD)] = '\0';

#define FAT_CLUSTER_CHAIN_END 0xFFFFFFFF
#define FAT_SECTOR_CHAIN_END 0xFFFFFFFF

/* Keep the entire FAT + metadata memory-resident. */
static sector_t partition_start_sector;
static struct fat_boot_record boot_record;
static cluster_t *fat;

static sector_t fat_cluster_offset_to_sector(cluster_t cluster, unsigned offset);
static sector_t fat_next_sector(sector_t sector);

bool fat_init(void) 
{
    if (!sd_init()) return false;

    struct mbr mbr;
    sd_sector_read(&mbr, 0);

    /* Find a FAT32 partition in the master boot record. I'm not sure
       if we can safely assume MBR layout on an SD card? They might
       have some MBR-less format, I think. */
    partition_start_sector = (unsigned) -1;
    for (int i = 0; i < 4; i++) {
        if (mbr.partition_entries[i].type_code == 0x0B ||
            mbr.partition_entries[i].type_code == 0x0C) {

            /* We just take the first one we see for now. */
            partition_start_sector = mbr.partition_entries[i].lba_begin;
            break;
        }
    }
    if (partition_start_sector == -1) {// No FAT32 partition found
        return false;
    }

    /* Read the FAT32 partition's boot record. */
    sd_sector_read(&boot_record, partition_start_sector);
#if 0
    STRINGIFY(oem);
    STRINGIFY(volume_label);
    STRINGIFY(system_id);
    printf("Found boot record:\n"
                   "System ID = '%s'\n"
                   "Volume Label = '%s'\n"
                   "OEM = '%s'\n",
           system_id, volume_label, oem);

    printf("Sectors per cluster: %d\n"
                   "Number of reserved sectors: %d\n"
                   "Sectors per FAT: %d\n"
                   "Media descriptor: %02x\n",
           boot_record.sectors_per_cluster,
           boot_record.num_reserved_sectors,
           boot_record.sectors_per_fat,
           boot_record.media_descriptor);
#endif

    /* Copy the (first copy of) the FAT into RAM. */
    fat = malloc(SD_SECTOR_SIZE * boot_record.sectors_per_fat);
    for (int i = 0; i < boot_record.sectors_per_fat; i++) {
        sd_sector_read(&fat[(SD_SECTOR_SIZE * i) / sizeof(*fat)],
                       partition_start_sector + boot_record.num_reserved_sectors + i);
    }
    return true;
}

cluster_t fat_root_cluster(void) {
    return boot_record.root_cluster;
}

static cluster_t fat_sector_to_cluster(sector_t sector) {
    const sector_t first_cluster_sector =
            partition_start_sector +
            boot_record.num_reserved_sectors +
            (boot_record.num_fats * boot_record.sectors_per_fat);

    return ((sector - first_cluster_sector) /
            boot_record.sectors_per_cluster) + 2;
}

static sector_t fat_cluster_offset_to_sector(cluster_t cluster, unsigned offset) {
    const sector_t first_cluster_sector =
            partition_start_sector +
            boot_record.num_reserved_sectors +
            (boot_record.num_fats * boot_record.sectors_per_fat);

    sector_t sector = first_cluster_sector + (cluster - 2) * boot_record.sectors_per_cluster;

    while (offset >= boot_record.sectors_per_cluster * SD_SECTOR_SIZE) {
        sector = fat_next_sector(sector);
        offset -= boot_record.sectors_per_cluster * SD_SECTOR_SIZE;
    }

    while (offset >= SD_SECTOR_SIZE) {
        sector++;
        offset -= SD_SECTOR_SIZE;
    }

    return sector;
}

static sector_t fat_next_sector(sector_t sector) {
    // Can we just get the next contiguous sector? We can, as long as
    // we aren't hitting the end of the cluster.
    if (fat_sector_to_cluster(sector) == fat_sector_to_cluster(sector + 1)) {
        return sector + 1;
    }

    // OK, we need to move on to the next cluster.
    // Do a FAT lookup.
    // FIXME: I don't think this works yet.
    cluster_t cluster = fat_sector_to_cluster(sector);

    if (fat[cluster] == FAT_CLUSTER_CHAIN_END)
        return FAT_SECTOR_CHAIN_END;

    return fat_cluster_offset_to_sector(fat[cluster], 0);
}


/* Helper function to read arbitrary byte ranges from the SD card's
   sector interface. */
unsigned fat_read(void *buf, cluster_t start_cluster, unsigned offset, unsigned len) {
    unsigned bufpos = 0;

    sector_t sector = fat_cluster_offset_to_sector(start_cluster, offset);
    unsigned sector_offset = offset % SD_SECTOR_SIZE;

    do {
        char temp[SD_SECTOR_SIZE];
        sd_sector_read(temp, sector);

        char *buf8 = (char *) buf;
        for (int i = sector_offset; i < SD_SECTOR_SIZE; i++) {
            if (bufpos >= len) {
                break;
            }

            buf8[bufpos++] = temp[i];
        }

        sector = fat_next_sector(sector);
        sector_offset = 0;
    } while (sector != FAT_SECTOR_CHAIN_END && bufpos < len);

    return bufpos;
}
