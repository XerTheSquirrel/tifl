// -*- Mode: Java; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

package net.multiphasicapps.tifl;

/**
 * This is used to create the framebuffer which may be attached to a native
 * image.
 *
 * @since 2017/01/11
 */
public interface FramebufferProvider
{
	/**
	 * Creates the framebuffer, must be ARGB.
	 *
	 * @param __w The framebuffer width.
	 * @param __h The framebuffer height.
	 * @return The data for the framebuffer.
	 * @since 2017/01/11
	 */
	public abstract int[] createFramebuffer(int __w, int __h);
}

