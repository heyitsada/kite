#include "../fmt.h"
#include "../sout.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  metadata_t meta = {0};
  bool have_meta = 0;
  bool have_rate = 0;
  bool have_chan = 0;
  bool have_bits = 0;

  /* stuff for header */
  char *meta_file = NULL, *input = NULL, *output = NULL;
  int sample_rate = 0;
  int channels = 0;
  int bits = 0;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
      meta_file = argv[++i];
      have_meta = 1;
    } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
      sample_rate = atoi(argv[++i]);
      have_rate = 1;
    } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
      channels = atoi(argv[++i]);
      have_chan = 1;
    } else if (strcmp(argv[i], "-b") == 0 && i + 1 < argc) {
      bits = atoi(argv[++i]);
      have_bits = 1;
    } else if (argv[i][0] != '-') {
      if (!input)
        input = argv[i];
      else if (!output)
        output = argv[i];
    }
  }

  if (!have_meta) {
    soutf("error: missing option -m\n");
    return 1;
  }
  if (!have_rate) {
    soutf("error: missing option -r\n");
    return 1;
  }
  if (!have_chan) {
    soutf("error: missing option -c\n");
    return 1;
  }
  if (!have_bits) {
    soutf("error: missing option -b\n");
    return 1;
  }

  if (sample_rate <= 0 || channels <= 0 || bits <= 0) {
    soutf("error: invalid sample rate, channels or bits\n");
    return 1;
  }

  if (!input) {
    soutf("error: missing input file\n");
    return 1;
  }

  /* da file */
  FILE *f = fopen(meta_file, "rb");
  if (!f) {
    soutf("error: could not open %s\n", meta_file);
    return 1;
  }
  if (fread(&meta, sizeof(meta), 1, f) != 1) {
    soutf("error: %s cannot be read\n", meta_file);
    fclose(f);
    return 1;
  }
  fclose(f);

  header_t header = {
      .version = 1,
      .sample_rate = (uint32_t)sample_rate,
      .channels = (uint32_t)channels,
      .bits = (uint32_t)bits,
      .metadata_offset = sizeof(header_t),
      .data_offset = sizeof(header_t) + sizeof(metadata_t),
  };
  memcpy(header.magic, MAGIC, 4);

  FILE *raw = fopen(input, "rb");
  if (!raw) {
    soutf("error: %s could not be opened\n", input);
    return 1;
  }

  FILE *out;
  if (output) {
    out = fopen(output, "wb");
    if (!out) {
      soutf("error: %s cannot be opened\n", output);
      fclose(raw);
      return 1;
    }
  } else {
    /* output goes to stdout */
    out = stdout;
  }

  if (fwrite(&header, sizeof(header), 1, out) != 1 ||
      fwrite(&meta, sizeof(meta), 1, out) != 1) {
    soutf("error: could not write header\n");
    return 1;
  }

  char buffer[8192];
  size_t n;

  while ((n = fread(buffer, 1, sizeof(buffer), raw)) > 0) {
    fwrite(buffer, 1, n, out);
  }

  fclose(raw);
  if (output)
    fclose(out);

  return 0;
}
