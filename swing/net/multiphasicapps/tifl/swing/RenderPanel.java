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

import java.awt.Dimension;
import java.awt.DisplayMode;
import java.awt.GraphicsConfiguration;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.Toolkit;
import javax.swing.JPanel;
import net.multiphasicapps.tifl.Rasterizer;

/**
 * This panel is used to render the game and contains the focus for it.
 *
 * @since 2017/01/11
 */
public class RenderPanel
	extends JPanel
{
	/**
	 * Initializes the render panel.
	 *
	 * @since 2017/01/11
	 */
	public RenderPanel()
	{
		// Never go below the intended resolution
		Dimension dim = new Dimension(
			Rasterizer.SCREEN_WIDTH, Rasterizer.SCREEN_HEIGHT);
		setMinimumSize(dim);
		setSize(dim);
		
		// The preferred size should be one that fits the size of the screen
		// the best because it will scale
		// For simplicity just use the resolution
		GraphicsEnvironment ge =
			GraphicsEnvironment.getLocalGraphicsEnvironment();
		GraphicsDevice gd = ge.getDefaultScreenDevice();
		DisplayMode dm = gd.getDisplayMode();
		
		// The current screen resolution
		double sw = dm.getWidth(), sh = dm.getHeight();
		
		// Ratio to simulate a lower desktop resolution
		double rw = sw / (Rasterizer.SCREEN_WIDTH * 1.5),
			rh = sh / (Rasterizer.SCREEN_HEIGHT * 1.5);
		
		// Calculate used size
		Dimension use = new Dimension();
		use.width = (int)(Rasterizer.SCREEN_WIDTH * rw);
		use.height = (int)(Rasterizer.SCREEN_HEIGHT * rh);
		
		// Use that size
		setPreferredSize(use);
	}
}

