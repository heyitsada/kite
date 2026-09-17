# Kite
Kite is a very simple audio container format that is focused on portability and being easy to implement

It has:
- Audio stream information
- Metadata
- RAW audio

## Format
The Kite format is structured as:
```
[Header][Metadata][Audio data]
```

The header has:
- Magic(KITE)
- sample rate
- channels
- bit depth
- offsets to metadata and audio

The metadata has:
- title
- artist
- album
- genre
- year
- track number
- disc number

## Tools
### kite-meta
Helps making metadata.

Example:
```sh
kite-meta \
    --title "God" \
    --artist "John Lennon" \
    --album "Plastic Ono Band" \
    --genre "Rock" \
    --year 1970 \
    --track 10 \
    --disc 1 \
    -o god.meta
```

### rawtokite
Helps making a raw file from a raw PCM audio and a metadata file.

Example:
```
rawtokite \
    -m god.meta \
    -r 48000 \
    -c 2 \
    -b 16 \
    god.raw \
    god.kite
```
No output specified and it goes to stdout
### kitetoraw
Extracts PCM audio from a Kite file.

Example:
```
kitetoraw god.kite god.raw
```

No output specified and it goes to stdout.
### kite-info
Shows the information of a Kite file.

Example:
```
kite-info god.kite
```

## Pipes
Kite is fully compatible with pipes.

Example using [shout](https://github.com/heyitsada/shout):
```
kitetoraw song.kite | shout -r 48000 -b 16 -c 2
```

## Goals
Kite tries to be:
- Friendly to Unix pipes
- Easy to implement(Especcially on osdev projects)
- Independent from any playback software.

## License
all the code is under public domain except from sout, which is MIT
