#ifndef FMT_H
#define FMT_H

#include <stdint.h>

/* For the benefit of mister kite...
 * there will be a show tonight on    \
 * trampoline. */
#define MAGIC "KITE"

typedef struct {
  char title[128];
  char artist[128];
  char album[128];
  char genre[64];

  uint16_t year;
  uint16_t track;
  uint16_t disc;
} metadata_t;

typedef struct {
  char magic[4];
  uint16_t version;

  /* The only reveltant info */
  uint32_t sample_rate;
  uint32_t channels;
  uint32_t bits;

  /* bloat */
  uint64_t metadata_offset;
  uint64_t data_offset;
} header_t;

#endif
