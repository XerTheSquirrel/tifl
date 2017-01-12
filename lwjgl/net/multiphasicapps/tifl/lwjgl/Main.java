// -*- Mode: Java; indent-tabs-mode: t; tab-width: 4 -*-
// ---------------------------------------------------------------------------
// Multi-Phasic Applications: Trouble In Furry Land
//     Copyright (C) Steven Gawroriski <steven@multiphasicapps.net>
//     Copyright (C) Multi-Phasic Applications <multiphasicapps.net>
// ---------------------------------------------------------------------------
// Trouble In Furry Land is under the GNU General Public License v3+, or later.
// See license.mkd for licensing and copyright information.
// ---------------------------------------------------------------------------

package net.multiphasicapps.tifl.lwjgl;

import org.lwjgl.LWJGLException;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;

/**
 * Main entry point for the Swing interface.
 *
 * @since 2017/01/11
 */
public class Main
	implements AutoCloseable, Runnable
{
	/**
	 * Instance of the main program.
	 *
	 * @param __args program arguments.
	 * @since 2017/01/12
	 */
	public Main(String... __args)
	{
		try
		{
			// Yay for purely static methods in classes
			Display.setDisplayMode(new DisplayMode(640, 480));
			
			// Create display
			Display.create();
		}
		
		// Failed
		catch (LWJGLException e)
		{
			throw new RuntimeException(e);
		}
	}
	
	/**
	 * {@inheritDoc}
	 * @since 2017/01/12
	 */
	@Override
	public void close()
	{
		// Destroy it
		Display.destroy();
	}
	
	/**
	 * {@inheritDoc}
	 * @since 2017/01/12
	 */
	@Override
	public void run()
	{
	}
	
	/**
	 * Main entry point.
	 *
	 * @param __args Program arguments.
	 * @since 2017/01/11
	 */
	public static void main(String... __args)
	{
		try (Main main = new Main(__args))
		{
			main.run();
		}
		
		GameFrame gf = new GameFrame(__args);
		gf.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		gf.setVisible(true);
	}
}

