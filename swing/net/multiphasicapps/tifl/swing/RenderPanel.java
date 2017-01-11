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
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GraphicsConfiguration;
import java.awt.GraphicsDevice;
import java.awt.GraphicsEnvironment;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.awt.Toolkit;
import javax.swing.JPanel;
import net.multiphasicapps.tifl.FramebufferProvider;
import net.multiphasicapps.tifl.Rasterizer;

/**
 * This panel is used to render the game and contains the focus for it.
 *
 * @since 2017/01/11
 */
public class RenderPanel
	extends JPanel
	implements FramebufferProvider
{
	/** Rasterizer for the game. */
	protected final Rasterizer rasterizer;
	
	/** Base buffer image. */
	protected final BufferedImage rasterimage;
	
	/** The raster size. */
	protected final int rasterwidth, rasterheight;
	
	/**
	 * Initializes the render panel.
	 *
	 * @since 2017/01/11
	 */
	public RenderPanel()
	{
		// The desired sized used
		int bw = Rasterizer.SCREEN_WIDTH,
			bh = Rasterizer.SCREEN_HEIGHT;
		
		// Never go below the intended resolution
		Dimension dim = new Dimension(bw, bh);
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
		double rw = sw / (bw * 1.5),
			rh = sh / (bh * 1.5);
		
		// Calculate used size
		Dimension use = new Dimension();
		use.width = (int)(bw * rw);
		use.height = (int)(bh * rh);
		
		// Use that size
		setPreferredSize(use);
		
		// Data is drawn directly onto this buffered image
		BufferedImage bi = new BufferedImage(bw, bh,
			BufferedImage.TYPE_INT_RGB);
		this.rasterimage = bi;
		
		// Setup rasterizer
		Rasterizer rasterizer = new Rasterizer(this, bw, bh);
		this.rasterizer = rasterizer;
		this.rasterwidth = bw;
		this.rasterheight = bh;
	}
	
	/**
	 * {@inheritDoc}
	 * @since 2017/01/11
	 */
	@Override
	public int[] createFramebuffer(int __w, int __h)
	{
		return ((DataBufferInt)this.rasterimage.getRaster().getDataBuffer()).
			getData();
	}
	
	/**
	 * {@inheritDoc}
	 * @since 2017/01/11
	 */
	@Override
	protected void paintComponent(Graphics __g)
	{
		super.paintComponent(__g);
		
		// Draw the game image
		__g.drawImage(this.rasterimage, 0, 0, getWidth(), getHeight(), null);
	}
}

