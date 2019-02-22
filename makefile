#!/usr/bin/make
# ---------------------------------------------------------------------------
# Multi-Phasic Applications: Trouble In Furry Land
#     Copyright (C) Stephanie Gawroriski <xer@multiphasicapps.net>
#     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
# ---------------------------------------------------------------------------
# Trouble In Furry Land is under the GNU General Public License v3+, or later.
# See license.mkd for licensing and copyright information.
# ---------------------------------------------------------------------------
# DESCRIPTION: Makefile for Trouble In Furry Land

SDL_CONFIG=sdl2-config
EXESUFFIX=

CFLAGS  := $(CFLAGS) $(shell $(SDL_CONFIG) --cflags) \
	-Werror=implicit-function-declaration
LDFLAGS := $(LDFLAGS) $(shell $(SDL_CONFIG) --libs) -lm

.PHONY: all
all:			tifl$(EXESUFFIX)
				

__objects := $(foreach __c,$(wildcard src/*.c),$(basename $(__c)).o)
__headers := $(wildcard src/*.h)

tifl$(EXESUFFIX):			$(__objects)
				$(CC) -o $@ $^ $(LDFLAGS)

%.o:			src/%.c $(__headers)
				$(CC) -o $@ -c $< $(CFLAGS)

clean:			
				rm -f -- $(__objects) tifl

