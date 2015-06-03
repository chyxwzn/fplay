#!/bin/sh
gcc -o fplayer ffplay.c cmdutils.c -I. -I/mingw64/include/SDL -lavformat -lavcodec -lavfilter -lavdevice -lswscale -lswresample -lavutil -lSDL -lm
