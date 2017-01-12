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
	/** Quit the game. */
	EVENTTYPE_QUIT,
	
	/** Number of event types. */
	NUM_EVENTTYPE
} EventType;

/**
 * This represents a game event.
 *
 * @since 2017/01/12
 */
typedef struct Event
{
	/** The type of event this is. */
	EventType type;
	
	/** The player the event is for. */
	int player;
	
	/** Event data. */
	union
	{
	} data;
} Event;

#endif

