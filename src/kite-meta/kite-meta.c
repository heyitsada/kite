#include "../fmt.h"
#include "../kite.h"
#include "../sout.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  metadata_t tmp = {0};

  /* flags for the given options */
  bool tofile = 0;
  bool error = 0;
  bool have_title = 0;
  bool have_artist = 0;
  bool have_album = 0;
  bool have_genre = 0;
  bool have_year = 0;
  bool have_track = 0;
  bool have_disc = 0;

  /* output file */
  char *output = NULL;

  /* handling flags */
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      output = argv[++i];
      tofile = 1;
    } else if (strcmp(argv[i], "--title") == 0 && i + 1 < argc) {
      strncpy(tmp.title, argv[++i], sizeof(tmp.title) - 1);
      tmp.title[sizeof(tmp.title) - 1] = '\0';
      have_title = 1;
    } else if (strcmp(argv[i], "--artist") == 0 && i + 1 < argc) {
      strncpy(tmp.artist, argv[++i], sizeof(tmp.artist) - 1);
      tmp.artist[sizeof(tmp.artist) - 1] = '\0';
      have_artist = 1;
    } else if (strcmp(argv[i], "--album") == 0 && i + 1 < argc) {
      strncpy(tmp.album, argv[++i], sizeof(tmp.album) - 1);
      tmp.album[sizeof(tmp.album) - 1] = '\0';
      have_album = 1;
    } else if (strcmp(argv[i], "--genre") == 0 && i + 1 < argc) {
      strncpy(tmp.genre, argv[++i], sizeof(tmp.genre) - 1);
      tmp.genre[sizeof(tmp.genre) - 1] = '\0';
      have_genre = 1;
    } else if (strcmp(argv[i], "--year") == 0 && i + 1 < argc) {
      tmp.year = (uint16_t)atoi(argv[++i]);
      have_year = 1;
    } else if (strcmp(argv[i], "--track") == 0 && i + 1 < argc) {
      tmp.track = (uint16_t)atoi(argv[++i]);
      have_track = 1;
    } else if (strcmp(argv[i], "--disc") == 0 && i + 1 < argc) {
      tmp.disc = (uint16_t)atoi(argv[++i]);
      have_disc = 1;
    } else {
      soutf("error: unexpected argument '%s'\n", argv[i]);
      error = 1;
    }
  }

  /* check that every option was provided */
  if (!have_title) {
    soutf("error: missing option --title\n");
    error = 1;
  }
  if (!have_artist) {
    soutf("error: missing option --artist\n");
    error = 1;
  }
  if (!have_album) {
    soutf("error: missing option --album\n");
    error = 1;
  }
  if (!have_genre) {
    soutf("error: missing option --genre\n");
    error = 1;
  }
  if (!have_year) {
    soutf("error: missing option --year\n");
    error = 1;
  }
  if (!have_track) {
    soutf("error: missing option --track\n");
    error = 1;
  }
  if (!have_disc) {
    soutf("error: missing option --disc\n");
    error = 1;
  }

  /* check that the values are sane */
  if (have_title && tmp.title[0] == '\0') {
    soutf("error: --title is empty\n");
    error = 1;
  }
  if (have_artist && tmp.artist[0] == '\0') {
    soutf("error: --artist is empty\n");
    error = 1;
  }
  if (have_album && tmp.album[0] == '\0') {
    soutf("error: --album is empty\n");
    error = 1;
  }
  if (have_genre && tmp.genre[0] == '\0') {
    soutf("error: --genre is empty\n");
    error = 1;
  }

  /* if something is wrong, lets print usage */
  if (error) {
    soutf("usage: kite-meta [options]\n"
          "\n"
          "options:\n"
          "  --title <text>       track title\n"
          "  --artist <text>      artist name\n"
          "  --album <text>       album name\n"
          "  --genre <text>       genre\n"
          "  --year <year>        release year\n"
          "  --track <number>     track number\n"
          "  --disc <number>      disc number\n"
          "  -o <file>            output metadata file\n"
          "\n"
          "without -o, metadata is written to stdout\n");
    return 1;
  }

  /* write metadata to file or stdout depending if -o exists */
  if (tofile) {
    FILE *f = fopen(output, "wb");
    if (!f) {
      return 1;
    }

    kite_write_meta(f, &tmp);
    fclose(f);
  } else {
    kite_write_meta(stdout, &tmp);
  }

  return 0;
}
