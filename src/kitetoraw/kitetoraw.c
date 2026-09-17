#include "../fmt.h"
#include "../sout.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  /* input file, output goes to stdout unless one is given */
  if (argc < 2 || argc > 3) {
    soutf("usage: kitetoraw <input.kite> [output.raw]\n");
    return 1;
  }

  header_t header = {0};

  FILE *f = fopen(argv[1], "rb");
  if (!f) {
    soutf("error: could not open %s\n", argv[1]);
    return 1;
  }

  /* header should have the magic */
  if (fread(&header, sizeof(header), 1, f) != 1) {
    soutf("error: %s cannot be read\n", argv[1]);
    fclose(f);
    return 1;
  }
  if (memcmp(header.magic, MAGIC, 4) != 0) {
    soutf("error: %s is not a kite file\n", argv[1]);
    fclose(f);
    return 1;
  }

  FILE *out;
  if (argc == 3) {
    out = fopen(argv[2], "wb");
    if (!out) {
      soutf("error: could not open %s\n", argv[2]);
      fclose(f);
      return 1;
    }
  } else {
    /* output goes to stdout */
    out = stdout;
  }

  /* audio data starts at data_offset */
  if (fseek(f, (long)header.data_offset, SEEK_SET) != 0) {
    soutf("error: %s has no audio data\n", argv[1]);
    fclose(f);
    if (argc == 3)
      fclose(out);
    return 1;
  }

  char buffer[8192];
  size_t n;

  while ((n = fread(buffer, 1, sizeof(buffer), f)) > 0) {
    fwrite(buffer, 1, n, out);
  }

  fclose(f);
  if (argc == 3)
    fclose(out);

  return 0;
}
