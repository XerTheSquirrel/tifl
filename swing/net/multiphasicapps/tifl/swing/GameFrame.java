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
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JFrame;
import javax.swing.Timer;
import net.multiphasicapps.tifl.Rasterizer;
import net.multiphasicapps.tifl.Simulation;

/**
 * This is the game frame which is used to show the actual game and
 * also potentially interact with it.
 *
 * @since 2017/01/11
 */
public class GameFrame
	extends JFrame
	implements Runnable
{
	/** The panel used to render the game. */
	protected final RenderPanel renderpanel;
	
	/** The game's rasterizer. */
	protected final Rasterizer rasterizer;
	
	/** The game loop thread. */
	protected final Thread gameloop;
	
	/** The current simulation being ran. */
	private volatile Simulation _simulation;
	
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
		
		// Setup rasterizer for drawing events
		this.rasterizer = renderpanel.rasterizer();
		
		// Setup game loop
		Thread gameloop = new Thread(this, "TIFLGameLoop");
		this.gameloop = gameloop;
		gameloop.setDaemon(true);
		
		// Start it
		gameloop.start();
	}
	
	/**
	 * {@inheritDoc}
	 * @since 2017/01/11
	 */
	@Override
	public void run()
	{
		// Infinite loop
		Rasterizer rasterizer = this.rasterizer;
		RenderPanel renderpanel = this.renderpanel;
		for (;;)
		{
			// Need to create the simulation?
			Simulation simulation = this._simulation;
			this._simulation = (simulation = new Simulation());
			
			// Draw this one
			rasterizer.setSimulation(simulation);
			
			// Run until it ends
			while (!simulation.hasEnded())
			{
				// Needed for timing
				long entertime = System.nanoTime();
				
				// Single frame
				simulation.runFrame();
				
				// Render it
				rasterizer.update();
				
				// Tell panel to update
				renderpanel.repaint();
				
				// Count time spent in the loop
				long exittime = System.nanoTime(),
					duration = exittime - entertime,
					rest = (Simulation.NANOS_PER_FRAME - duration) / 1_000L;
				
				// Sleep?
				if (rest > 0)
					try
					{
						Thread.sleep(rest);
					}
					catch (InterruptedException e)
					{
						// Ignore
					}
			}
		}
	}
}

