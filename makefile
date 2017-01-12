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

CFLAGS  := $(CFLAGS) $(shell $(SDL_CONFIG) --cflags)
LDFLAGS := $(LDFLAGS) $(shell $(SDL_CONFIG) --libs)

.PHONY: all
all:			tifl
				

__objects := $(foreach __c,$(wildcard src/*.c),$(basename $(__c)).o)

tifl:			$(__objects)
				$(CC) -o $@ $< $(LDFLAGS)

%.o:			src/%.c
				$(CC) -o $@ -c $< $(CFLAGS)

