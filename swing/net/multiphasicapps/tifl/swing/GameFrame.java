// -*- Mode: Java; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

package net.multiphasicapps.tifl.swing;

import java.awt.BorderLayout;
import javax.swing.JFrame;

/**
 * This is the game frame which is used to show the actual game and
 * also potentially interact with it.
 *
 * @since 2017/01/11
 */
public class GameFrame
	extends JFrame
{
	/** The panel used to render the game. */
	protected final RenderPanel renderpanel;
	
	/**
	 * Initializes the game frame.
	 *
	 * @param __args The program arguments.
	 * @since 2017/01/11
	 */
	public GameFrame(String... __args)
	{
		super("Trouble In Furry Land");
		
		// Setup the render panel
		RenderPanel renderpanel = new RenderPanel();
		this.renderpanel = renderpanel;
		
		// Add it to the frame
		add(renderpanel, BorderLayout.CENTER);
		
		// Pack and center to make it neat
		pack();
		setLocationRelativeTo(null);
	}
}

