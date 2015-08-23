#!/bin/sh
gcc -o player ffplay.c cmdutils.c libavfilter/drawutils.c libavfilter/formats.c ffplay_dxva2.c -I. -I/mingw64/include/SDL -lavformat -lavcodec -lavfilter -lavdevice -lswscale -lswresample -lavutil -lSDL -lSDL_ttf -lm -lass -lole32
