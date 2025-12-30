# C PONG

A simple Pong clone written in C as an excercise in learning the language. Comes with WIN32 and WASAPI support. Currently only supports Windows.

## Current issues and changes needed:
- Better memory management (arenas need introducing).

### Compile with:
gcc *.c -o app.exe -lgdi32 -lwinmm -lole32 -luuid -lm -pthread
