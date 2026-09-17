#include "kite.h"

#include <stdint.h>
#include <string.h>

static void put_u16le(uint8_t *p, uint16_t v) {
  p[0] = v & 0xff;
  p[1] = v >> 8;
}

static uint16_t get_u16le(const uint8_t *p) {
  return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static void put_u32le(uint8_t *p, uint32_t v) {
  p[0] = v & 0xff;
  p[1] = (v >> 8) & 0xff;
  p[2] = (v >> 16) & 0xff;
  p[3] = (v >> 24) & 0xff;
}

static uint32_t get_u32le(const uint8_t *p) {
  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) |
         ((uint32_t)p[3] << 24);
}

static void put_u64le(uint8_t *p, uint64_t v) {
  for (int i = 0; i < 8; i++)
    p[i] = (uint8_t)(v >> (8 * i));
}

static uint64_t get_u64le(const uint8_t *p) {
  uint64_t v = 0;

  for (int i = 0; i < 8; i++)
    v |= (uint64_t)p[i] << (8 * i);

  return v;
}

int kite_write_header(FILE *f, const header_t *h) {
  uint8_t buf[KITE_HEADER_SIZE] = {0};

  memcpy(buf, h->magic, 4);
  put_u16le(buf + 4, h->version);
  put_u32le(buf + 6, h->sample_rate);
  put_u32le(buf + 10, h->channels);
  put_u32le(buf + 14, h->bits);
  put_u64le(buf + 18, h->metadata_offset);
  put_u64le(buf + 26, h->data_offset);

  return fwrite(buf, 1, KITE_HEADER_SIZE, f) == KITE_HEADER_SIZE;
}

int kite_read_header(FILE *f, header_t *h) {
  uint8_t buf[KITE_HEADER_SIZE];

  if (fread(buf, 1, KITE_HEADER_SIZE, f) != KITE_HEADER_SIZE)
    return 0;

  memcpy(h->magic, buf, 4);
  h->version = get_u16le(buf + 4);
  h->sample_rate = get_u32le(buf + 6);
  h->channels = get_u32le(buf + 10);
  h->bits = get_u32le(buf + 14);
  h->metadata_offset = get_u64le(buf + 18);
  h->data_offset = get_u64le(buf + 26);

  return 1;
}

int kite_write_meta(FILE *f, const metadata_t *m) {
  uint8_t buf[KITE_META_SIZE] = {0};

  memcpy(buf, m->title, 128);
  memcpy(buf + 128, m->artist, 128);
  memcpy(buf + 256, m->album, 128);
  memcpy(buf + 384, m->genre, 64);
  put_u16le(buf + 448, m->year);
  put_u16le(buf + 450, m->track);
  put_u16le(buf + 452, m->disc);

  return fwrite(buf, 1, KITE_META_SIZE, f) == KITE_META_SIZE;
}

int kite_read_meta(FILE *f, metadata_t *m) {
  uint8_t buf[KITE_META_SIZE];

  if (fread(buf, 1, KITE_META_SIZE, f) != KITE_META_SIZE)
    return 0;

  memcpy(m->title, buf, 128);
  memcpy(m->artist, buf + 128, 128);
  memcpy(m->album, buf + 256, 128);
  memcpy(m->genre, buf + 384, 64);
  m->year = get_u16le(buf + 448);
  m->track = get_u16le(buf + 450);
  m->disc = get_u16le(buf + 452);

  return 1;
}
