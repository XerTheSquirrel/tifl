// -*- Mode: C; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

#ifndef TIFL_EVENT_H
#define TIFL_EVENT_H

/**
 * This represents the event type.
 *
 * @since 2017/01/12
 */
typedef enum EventType
{
	/** No event. */
	EVENTYPE_NONE,	
	
	/** Quit the game. */
	EVENTTYPE_QUIT,
	
	/** Turn left. */
	EVENTTYPE_TURN_LEFT,
	
	/** Turn right. */
	EVENTTYPE_TURN_RIGHT,
	
	/** Number of event types. */
	NUM_EVENTTYPE
} EventType;

/** Keys which are held down. */
extern boolean gamekeydown[NUM_EVENTTYPE];

#endif

