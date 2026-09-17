#include "../fmt.h"
#include "../kite.h"
#include "../sout.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  /* filename is the only argument we need */
  if (argc != 2) {
    soutf("usage: kite-info <file.kite>\n");
    return 1;
  }

  header_t header = {0};
  metadata_t meta = {0};

  FILE *f = fopen(argv[1], "rb");
  if (!f) {
    soutf("error: could not open %s\n", argv[1]);
    return 1;
  }

  /* header must have the magic */
  if (!kite_read_header(f, &header)) {
    soutf("error: %s cannot be read\n", argv[1]);
    fclose(f);
    return 1;
  }

  if (memcmp(header.magic, MAGIC, 4) != 0) {
    soutf("error: %s is not a kite file\n", argv[1]);
    fclose(f);
    return 1;
  }

  /* metadata lives at metadata_offset */
  if (fseek(f, (long)header.metadata_offset, SEEK_SET) != 0 ||
      !kite_read_meta(f, &meta)) {
    soutf("error: %s cannot be read\n", argv[1]);
    fclose(f);
    return 1;
  }
  fclose(f);

  /* metadata */
  soutf("TITLE=\"%s\"\n", meta.title);
  soutf("ARTIST=\"%s\"\n", meta.artist);
  soutf("ALBUM=\"%s\"\n", meta.album);
  soutf("GENRE=\"%s\"\n", meta.genre);
  soutf("YEAR=\"%d\"\n", meta.year);
  soutf("TRACK=\"%d\"\n", meta.track);
  soutf("DISC=\"%d\"\n", meta.disc);

  /* audio info */
  soutf("SAMPLE_RATE=\"%d\"\n", header.sample_rate);
  soutf("CHANNELS=\"%d\"\n", header.channels);
  soutf("BITS=\"%d\"\n", header.bits);

  return 0;
}
