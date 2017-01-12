// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#include "global.h"

#include "sine.h"
#include "atan.h"


/* FixedMul() -- Multiplies two fixed point numbers */
fixedtype FixedMul(const fixedtype a_A, const fixedtype a_B)
{
	return ((int64_t)a_A * (int64_t)a_B) >> (int64_t)FIXEDSHIFT;
}

/* FixedDiv() -- Divides two fixed point numbers */
fixedtype FixedDiv(const fixedtype a_A, const fixedtype a_B)
{
	return ((int64_t)a_A << (int64_t)FIXEDSHIFT) / (int64_t)a_B;
}

/* OctoDist() -- Octogonal distance calculation */
int32_t OctoDist(const int32_t a_X1, const int32_t a_Y1, const int32_t a_X2, const int32_t a_Y2)
{
	int32_t dx, dy, mm;	
	
	/* Distance from xy */
	dx = a_X2 - a_X1;
	if (dx < 0)
		dx = -dx;
	dy = a_Y2 - a_Y1;
	if (dy < 0)
		dy = -dy;
	
	/* Max */
	if (dx > dy)
		mm = dx;
	else
		mm = dy;
	
	/* Caluclate difference */
	return ((dx + dy) + mm) >> 1;
}

/* AngleSin() -- Sine of angle */
fixedtype AngleSin(const angletype a_An)
{
	angletype NBits;
	
	/* Get lower angle bits */
	NBits = (a_An & (ANG90 - 1)) >> 15;
	
	/* 0 - 180 */
	if (a_An < ANG180)
		// 0 - 90: Approaches 1
		if (a_An < ANG90)
			return (fixedtype)sinetable[NBits];
		
		// 90 - 180: Approaches zero
		else
			return FIXEDONE - (fixedtype)sinetable[NBits];
	
	/* 180 - 360 */
	else
		// 180 - 270: Approaches negative 1
		if (a_An < ANG270)
			return -((fixedtype)sinetable[NBits]);
		
		// 270 - 360: Approaches zero
		else
			return -((fixedtype)sinetable[32768 - NBits]);
}

/* AngleCos() -- Cosine of angle */
fixedtype AngleCos(const angletype a_An)
{
	return AngleSin(a_An + ANG90);
}

/* AngleDiv() -- Angle divisor */
static inline uint32_t AngleDiv(const int32_t a_N, const int32_t a_D)
{
	return ((((uint64_t)a_N) << UINT64_C(32)) /
		((uint64_t)a_D)) >> UINT64_C(16);
}

/* RiseRunDiv() -- Rise over run division */
// This should result in ATANSIZE values
static inline uint32_t RiseRunDiv(const int32_t a_A, const int32_t a_B)
{
	int64_t Div;
	
	/* Divide entire number */
	// This creates a 32.32 fixed point number
	Div = ((((int64_t)a_A) << UINT64_C(32)) / (int64_t)a_B);
	
	// Down shift into 4096 range
	Div >>= 20;
	
	if (Div >= ATANSIZE)
		return ATANSIZE - 1;
	else if (Div < 0)
		return 0;
	else
		return Div;
}

/* LineToAngle() -- Gets angle from line */
// Uses the mighty SOH CAH TOA
//    /|
//   / | rise
//  *--|
//   run   * = (0, 0)
// angle (in rads) = arctan(rise / run)
angletype LineToAngle(const int32_t a_X1, const int32_t a_Y1,
	const int32_t a_X2, const int32_t a_Y2)
{
	int32_t dx, dy;
	
	/* Difference of points */
	dx = a_X2 - a_X1;
	dy = a_Y2 - a_Y1;
	
	/* Same point is angle zero */
	if (!dx && !dy)
		return 0;
	
	/* A nice picture */
	//              90°
	//        `  3  ^  2   ,
	//         \    |    /
	//       4   \  |  /
	//             \|/     1
	// 180° <-------+-------> 0°
	//             /|\    8
	//       5   /  |  \
	//         /  6 v 7  \      .
	//        ,    270°   `
	
	/* Negative x: 3 4 5 6 */
	if (dx < 0)
	{
		// Flip
		dx = -dx;
		
		// Negative y: 5 6
		if (dy < 0)
		{
			// Flip
			dy = -dy;
			
			// < y: 5
			if (dy < dx)
				return ANG180 + atanlut[RiseRunDiv(dy, dx)];
			
			// > y: 6
			else
				return (ANG270 - 1) - atanlut[RiseRunDiv(dx, dy)];
		}
		
		// Positive y: 3 4
		else
		{
			// < y: 4
			if (dy < dx)
				return (ANG180 - 1) - atanlut[RiseRunDiv(dy, dx)];
			
			// > y: 3
			else
				return ANG90 + atanlut[RiseRunDiv(dx, dy)];
		}
	}
	
	/* Positive x: 1 2 8 7 */
	else
	{
		// Negative y: 7 8
		if (dy < 0)
		{
			// Flip
			dy = -dy;
			
			// < y: 8
			if (dy < dx)
				return ANG0 - atanlut[RiseRunDiv(dy, dx)];
			
			// > y: 7
			else
				return ANG270 + atanlut[RiseRunDiv(dx, dy)];
		}
		
		// Positive y: 1 2
		else
		{
			// < y: 1
			if (dy < dx)
				return atanlut[RiseRunDiv(dy, dx)];
			
			// > y: 2
			else
				return (ANG90 - 1) - atanlut[RiseRunDiv(dx, dy)];
		}
	}
}

