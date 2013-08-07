# Visualization code for simulation.
# Adapted from MIT 6.00 problem set 11

import math
import time

from Tkinter import *

class antVisualization:
    def __init__(self, antWorld, delay = 0.05):
        "Initializes a visualization with the specified parameters."
        # Number of seconds to pause after each frame
        self.delay = delay

        self.max_dim = max(antWorld.getWorldWidth(), antWorld.getWorldHeight())
        self.width = antWorld.getWorldWidth()
        self.height = antWorld.getWorldHeight()
 
        # Initialize a drawing surface
        self.master = Tk()
        self.w = Canvas(self.master, width=500, height=500)
        self.w.pack()
        self.master.update()

        # Draw a backing and lines
        r1, c1 = self._map_coords(0, 0)
        r2, c2 = self._map_coords(self.width, self.height)
        self.w.create_rectangle(c1, r1, c2, r2, fill = "white")

	# Draw the world. Black square for the ant mound
	# 
        self.tiles = {}
        for row in range(antWorld.getWorldHeight()):
            for col in range(antWorld.getWorldWidth()):
                if not antWorld.getTile(row, col).isTravelable():
                    r1, c1 = self._map_coords(row, col)
                    r2, c2 = self._map_coords(row + 1, col + 1)
                    self.w.create_rectangle(c1, r1, c2, r2, fill = "gray")
                if antWorld.getTile(row, col).getAmountOfFood() != 0:
                    f1, f2 = self._scaleFood(antWorld.getTile(row, col).getAmountOfFood())
                    r1, c1 = self._map_coords(row + f1, col + f1)
                    r2, c2 = self._map_coords(row + f2, col + f2)
                    self.tiles[(row, col)] = self.w.create_oval(c1, r1, c2, r2, fill = "yellow")
                if antWorld.getTile(row, col).isAntHome():
                    r1, c1 = self._map_coords(row, col)
                    r2, c2 = self._map_coords(row + 1, col + 1)
                    self.w.create_oval(c1, r1, c2, r2, fill = "brown")
                    self.homeRow, self.homeCol = row, col

                
        # Draw gridlines
        for i in range(self.width + 1):
            r1, c1 = self._map_coords(i, 0)
            r2, c2 = self._map_coords(i, self.height)
            self.w.create_line(c1, r1, c2, r2)
        for i in range(self.height + 1):
            r1, c1 = self._map_coords(0, i)
            r2, c2 = self._map_coords(self.width, i)
            self.w.create_line(c1, r1, c2, r2)

        # Draw some status text
        self.ants = None
        self.text = self.w.create_text(25, 0, anchor=NW,
                                       text=self._toString(0, 0))
        self.time = 0
        self.master.update()

    def _toString(self, time, numFood):
        "Returns an appropriate status string to print."
        
        return "Time: %04d; %d food units deposited at ant mound" % (time, numFood)

    def _map_coords(self, row, col):
        "Maps grid positions to window positions (in pixels)."
        return (250 + 450 * ((row - self.height / 2.0) / self.max_dim),
                250 + 450 * ((col - self.width / 2.0) / self.max_dim))

    def _scaleFood(self, scaleFactor):
        """Returns the numbers to scale the oval representing food"""
        return (0.5 - 0.5*(scaleFactor/5.), 0.5 + 0.5*(scaleFactor/5.))
    
    def update(self, antWorld):
        "Redraws the visualization with the specified room and ant state."

        ## deletes all existing food
        for key in self.tiles.keys():
            self.w.delete(self.tiles[key])

        for key in self.tiles.keys():
            row, col = key
            if antWorld.getTile(row, col).getAmountOfFood() > 0:
                f1, f2 = self._scaleFood(antWorld.getTile(row, col).getAmountOfFood())
                r1, c1 = self._map_coords(row + f1, col + f1)
                r2, c2 = self._map_coords(row + f2, col + f2)
                self.tiles[(row, col)] = self.w.create_oval(c1, r1, c2, r2, fill = "yellow")


        # Delete all existing robots.
        if self.ants:
            for ant in self.ants:
                self.w.delete(ant)
                self.master.update_idletasks() 

        # Draws an ant on any tile were ants are present
        self.ants = []
        for row in xrange(antWorld.getWorldHeight()):
            for col in range(antWorld.getWorldWidth()):
                if antWorld.getTile(row,col).getAnts() != []:
                    r1, c1 = self._map_coords(0.5 + row - 0.08, 0.5 + col - 0.08)
                    r2, c2 = self._map_coords(0.5 + row + 0.08, 0.5 + col + 0.08)
                    self.ants.append(self.w.create_oval(c1, r1, c2, r2, fill = "black"))
                                     

        # Update text
        self.w.delete(self.text)
        self.time += 1
        self.text = self.w.create_text( \
            25, 0, anchor=NW, \
            text=self._toString( self.time, antWorld.getTile(self.homeRow,self.homeCol).getAmountOfFood() ))
        self.master.update()
        time.sleep(self.delay)

    def done(self):
        "Indicate that the animation is done so that we allow the user to close the window."
        mainloop()

