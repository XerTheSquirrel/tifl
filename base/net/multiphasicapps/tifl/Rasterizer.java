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
 * This contains the rasterizer for the game which renders it.
 *
 * @since 2017/01/11
 */
public class Rasterizer
{
	/** The screen width. */
	public static final int SCREEN_WIDTH =
		320;
	
	/** The screen height. */
	public static final int SCREEN_HEIGHT =
		200;
	
	/** The game framebuffer. */
	protected final int[] framebuffer;
	
	/** The view width. */
	protected final int width;
	
	/** The view height. */
	protected final int height;
	
	/**
	 * Initializes the rasterizer with the default resolution.
	 *
	 * @since 2017/01/11
	 */
	public Rasterizer()
	{
		this(SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	
	/**
	 * Initializes the rasterizer with the given resolution.
	 *
	 * @param __w The width.
	 * @param __h The height.
	 * @throws IllegalArgumentException If any dimension is zero or negative.
	 * @since 2017/01/11
	 */
	public Rasterizer(int __w, int __h)
		throws IllegalArgumentException
	{
		this(new FramebufferProvider()
			{
				/**
				 * {@inheritDoc}
				 * @since 2017/01/11
				 */
				@Override
				public int[] createFramebuffer(int __w, int __h)
				{
					return new int[__w * __h];
				}
			}, __w, __h);
	}
	
	/**
	 * Initializes the rasterizer with the default resolution and the given
	 * framebuffer provider.
	 *
	 * @param __fbp The framebuffer provider to use.
	 * @throws NullPointerException On null arguments.
	 * @since 2017/01/11
	 */
	public Rasterizer(FramebufferProvider __fbp)
		throws NullPointerException
	{
		this(__fbp, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	
	/**
	 * Initializes the rasterizer with the given resolution and the given
	 * framebuffer provider.
	 *
	 * @param __fbp The framebuffer provider to use.
	 * @param __w The width.
	 * @param __h The height.
	 * @throws IllegalArgumentException If any dimension is zero or negative.
	 * @throws NullPointerException On null arguments.
	 * @since 2017/01/11
	 */
	public Rasterizer(FramebufferProvider __fbp, int __w, int __h)
		throws IllegalArgumentException, NullPointerException
	{
		// Check
		if (__fbp == null)
			throw new NullPointerException();
		if (__w <= 0 || __h <= 0)
			throw new IllegalArgumentException();
		
		// Set
		this.width = __w;
		this.height = __h;
		
		// Setup framebuffer
		int[] framebuffer = __fbp.createFramebuffer(__w, __h);
		this.framebuffer = framebuffer;
		
		// Initialize framebuffer with something, to see how it works
		double br = 255.0 / __w, at = 0.0;
		for (int i = 0, p = (__w * __h); i < p; i++)
		{
			int bid = ((int)at) & 0xFF;
			framebuffer[i] = (bid << 16) | (bid << 8) | (bid);
			
			// Switch directions?
			at += br;
			if (((i + 1) % __w) == 0)
				br = -br;
		}
	}
	
	/**
	 * Returns the raw framebuffer.
	 *
	 * @return The framebuffer
	 * @since 2017/01/11
	 */
	public int[] framebuffer()
	{
		return this.framebuffer;
	}
	
	/**
	 * Returns the height.
	 *
	 * @return The height.
	 * @since 2017/01/11
	 */
	public int height()
	{
		return this.height;
	}
	
	/**
	 * Returns the width.
	 *
	 * @return The width.
	 * @since 2017/01/11
	 */
	public int width()
	{
		return this.width;
	}
}

