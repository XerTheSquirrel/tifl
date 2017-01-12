// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#ifndef TIFL_GLOBAL_H
#define TIFL_GLOBAL_H

#include <stdint.h>

/** The number of frames per second in the game. */
#define GAME_FRAMES_PER_SECOND 20

/** Milliseconds per game frame. */
#define MILLISECONDS_PER_FRAME 50

/**
 * Boolean type.
 *
 * @since 2017/01/12
 */
typedef enum
{
	false,
	true
} boolean;

#define FIXEDSHIFT ((int32_t)16)
#define FIXED_C(x) (INT32_C(x) << FIXEDSHIFT)
#define FIXEDONE FIXED_C(1)
#define FIXEDHALF ((int32_t)INT32_C(0x8000))
#define FIXEDMASK ((int32_t)INT32_C(0xFFFF))

#define fxtoi(x) (((int32_t)(x)) >> FIXEDSHIFT)
#define itofx(x) (((int32_t)(x)) << FIXEDSHIFT)

typedef int32_t fixedtype;

fixedtype FixedMul(const fixedtype a_A, const fixedtype a_B);
fixedtype FixedDiv(const fixedtype a_A, const fixedtype a_B);
int32_t OctoDist(const int32_t a_X1, const int32_t a_Y1,
	const int32_t a_X2, const int32_t a_Y2);

#define ANGLETYPE_C(x) (UINT32_C(x))

#define ANG0	ANGLETYPE_C(0x00000000)
#define ANG1	ANGLETYPE_C(0x00b60b60)
#define ANG45	ANGLETYPE_C(0x20000000)
#define ANG60   ANGLETYPE_C(0x2AAAAA80)
#define ANG90	ANGLETYPE_C(0x40000000)
#define ANG135	ANGLETYPE_C(0x60000000)
#define ANG180	ANGLETYPE_C(0x80000000)
#define ANG225	ANGLETYPE_C(0xA0000000)
#define ANG270	ANGLETYPE_C(0xC0000000)
#define ANG315	ANGLETYPE_C(0xE0000000)

typedef uint32_t angletype;

fixedtype AngleSin(const angletype a_An);
fixedtype AngleCos(const angletype a_An);
fixedtype AngleTan(const angletype a_An);

angletype LineToAngle(const int32_t a_X1, const int32_t a_Y1,
	const int32_t a_X2, const int32_t a_Y2);

/**
 * Kills the program.
 *
 * @param format Formatted text.
 * @since 2017/01/12
 */
void Die(const char* format, ...);

#endif

