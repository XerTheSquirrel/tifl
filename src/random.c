// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#include "random.h"

static int64_t value;

void SeedRandom(int64_t seed)
{
	value = (seed ^ UINT64_C(0x5DEECE66D)) & ((UINT64_C(1) << 48) - 1);
}

int32_t NextRandom()
{
	value = (value * UINT64_C(0x5DEECE66D) + UINT64_C(0xB)) &
		((UINT64_C(1) << 48) - 1);
	
	return (int32_t)(value >> 16);
}

