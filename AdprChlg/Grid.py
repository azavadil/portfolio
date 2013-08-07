class Grid(object): 
    """ Grid class implementation
        The layout is in row-major order, which is to say that
        the first entire row is laid out contiguously, followed by the entire
	next row and so on. All access is bounds-checked for safety.
    """
	
    def __init__(self, rows, cols, initValue = 0):

        self.nRows = rows
        self.nCols = cols
        self.data = [[initValue for col in xrange(cols)] for row in xrange(rows)]

    def inBounds(self, row, col):
        return row >= 0 and col >= 0 and row < self.nRows and col < self.nCols

    def setAt(self, row, col, elemValue):
        self.data[row][col] = elemValue

    def getAt(self, row, col):
        return self.data[row][col]
    
    def resize(self, newRows, newCols, initValue = 0):
        """
        resizes the grid by the specified number of rows and cols
        copying the existing data into the center space
        ##########
        #        #
        #  ####  #
        #  #  #  #
        #  #  #  #
        #  ####  #
        #        #
        ##########
        """

        oldRows = self.nRows
        oldCols = self.nCols
        self.nRows += 2*newRows
        self.nCols += 2*newCols
        copy = [[initValue for col in xrange(self.nCols)] for row in xrange(self.nRows)]

        for row in xrange(newRows, newRows + oldRows): 
            for col in xrange(newCols, newCols + oldCols):
                copy[row][col] = self.getAt(row - newRows, col - newCols)

        self.data = copy

    def getRows(self):
        return self.nRows

    def getCols(self):
        return self.nCols

    def showGrid(self):
        for i in xrange(self.nRows):
            print self.data[i]
