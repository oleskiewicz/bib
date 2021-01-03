VERSION   = 0.1
CC        = cc
LD        = $(CC)
PREFIX    = /usr/local
MANPREFIX = $(PREFIX)/share
CFLAGS    = -DVERSION=\"$(VERSION)\" -Wall -std=c99 -Os
LDLIBS    = -lbtparse
