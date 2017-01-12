// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#include <SDL.h>
#include <SDL_stdinc.h>
#include <stdio.h>
#include <stdarg.h>

#include "global.h"
#include "video.h"

void Die(const char* format, ...)
{
#define DIE_SIZE 512
	char die[DIE_SIZE];
	va_list args;
	
	// Format it
	va_start(args, format);
	SDL_vsnprintf(die, DIE_SIZE - 1, format, args);
	die[DIE_SIZE - 1] = 0;
	va_end(args);
	
	// Print to output
	fprintf(stderr, "%s\n", format);
	abort();
#undef DIE_SIZE
}

/**
 * Main entry point.
 *
 * @param argc Argument count.
 * @param argv Arguments.
 * @return 0 on success, anything else on failure.
 * @since 2017/01/12
 */
int main(int argc, char** argv)
{
	// Initialize events
	if (SDL_Init(SDL_INIT_EVENTS) != 0)
		Die("Failed to initialize event handler.");
	
	// Initialize video
	if (VideoInit() != 0)
		Die("Failed to initialize video.");
	
	return 0;
}

