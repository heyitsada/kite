#ifndef KITE_H
#define KITE_H

#include "fmt.h"

#include <stdio.h>

#define KITE_HEADER_SIZE 34
#define KITE_META_SIZE 454

int kite_write_header(FILE *f, const header_t *h);
int kite_read_header(FILE *f, header_t *h);
int kite_write_meta(FILE *f, const metadata_t *m);
int kite_read_meta(FILE *f, metadata_t *m);

#endif
