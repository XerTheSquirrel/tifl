// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#ifndef TIFL_RANDOM_H
#define TIFL_RANDOM_H

#include "global.h"

/**
 * Seeds the random number generator.
 *
 * @param seed The seed used.
 * @since 2017/01/15
 */
void SeedRandom(int64_t seed);

/**
 * Returns the next random number.
 *
 * @return The next random number.
 * @since 2017/01/15
 */
int32_t NextRandom();

#endif

