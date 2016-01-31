#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "elf.h"

static const char *prog_name;

void error_exit(const char *msg) {
  fprintf(stderr, "Error: %s\n", msg);
  fprintf(stderr, "Usage: %s <input-file> <output-file>\n", prog_name);
  exit(1);
}

typedef struct BinaryOutput {
  size_t length;
  uint32_t *data;
  bool needs_free;
} bin_t;

bin_t *obj_to_binary(struct Elf *elf, bin_t *output) {
  if (elf->e_magic != ELF_MAGIC) {
    printf("ELF Magic Mismatch.\n");
    return NULL;
  }

  if (!elf->e_shoff) {
    printf("This file has no section header.\n");
    return NULL;
  }

  uintptr_t sect_hdr_addr = ((uintptr_t) elf) + elf->e_shoff;
  struct Secthdr *sections = (struct Secthdr *)sect_hdr_addr;
  struct Secthdr *sh_str_sect = sections + elf->e_shstrndx;
  char *str_tab = (char *)((uintptr_t)(elf) + sh_str_sect->sh_offset);

  int i = 0;
  struct Secthdr *sect;
  for (sect = sections; sect < sections + elf->e_shnum; sect++) {
    char *name = &str_tab[sect->sh_name];
    printf("[%d] %s (%d)\n", i++, name, sect->sh_type);
    if (sect->sh_type == ELF_SHT_PROGBITS) {
      output->data = (uint32_t *)(((uintptr_t)elf) + sect->sh_offset);
      output->length = sect->sh_size;
      output->needs_free = false;
      return output;
    }
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  prog_name = argv[0];
  if (argc < 3) {
    error_exit("Wrong number of arguments.");
  }

  // Open the file for reading.
  const char *input = argv[1];
  int fd = open(input, O_RDONLY);
  if (fd == -1) {
    error_exit("Couldn't open the input file for reading.");
  }

  // mmap() the file.
  struct stat st;
  stat(input, &st);
  struct Elf *input_file = (struct Elf *)mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

  // Attempt to do the conversion now. If it succeeds, try to write out the
  // file. If it fails, print a message.
  bin_t binary_out;
  if (obj_to_binary(input_file, &binary_out)) {
    const char *output = argv[2];
    FILE *out_file = fopen(output, "w");
    if (fwrite(binary_out.data, binary_out.length, 1, out_file) != 1) {
      fprintf(stderr, "Failed to write data out.");
    }

    fclose(out_file);
    if (binary_out.needs_free) free(binary_out.data);
  } else {
    fprintf(stderr, "Failed to create binary object.\n");
  }

  // Final cleanup.
  munmap(input_file, st.st_size);
  close(fd);
  return 0;
}
