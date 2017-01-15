#!/usr/bin/make
# ---------------------------------------------------------------------------
# Multi-Phasic Applications: Trouble In Furry Land
#     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
#     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
# ---------------------------------------------------------------------------
# Trouble In Furry Land is under the GNU General Public License v3+, or later.
# See license.mkd for licensing and copyright information.
# ---------------------------------------------------------------------------
# DESCRIPTION: Makefile for Trouble In Furry Land

SDL_CONFIG=sdl2-config

CFLAGS  := $(CFLAGS) $(shell $(SDL_CONFIG) --cflags) -g3 -O0 \
	-Werror=implicit-function-declaration -pg
LDFLAGS := $(LDFLAGS) $(shell $(SDL_CONFIG) --libs) -lm -pg

.PHONY: all
all:			tifl
				

__objects := $(foreach __c,$(wildcard src/*.c),$(basename $(__c)).o)
__headers := $(wildcard src/*.h)

tifl:			$(__objects)
				$(CC) -o $@ $^ $(LDFLAGS)

%.o:			src/%.c $(__headers)
				$(CC) -o $@ -c $< $(CFLAGS)

clean:			
				rm -f -- $(__objects) tifl

