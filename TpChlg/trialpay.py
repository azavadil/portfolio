
def snake_cube_solution(X, Y, Z, puzzleString, printOn=False):
    """
    Returns 1 if there is a valid solution and 0 is if no solution exists.
    If the printOn paramenter equals True, snake_cube_solution will print
    the (x,y,z) coordinate and the orientation of each cubelet.

    snake_cube_solution tkaes the dimensions of a puzzle (X, Y, Z) and
    a string representation where 0 represents a straight junction and
    1 represents a flexible junction
    """
    
    ## convert string to list
    puzzle = list(puzzleString)

    ## check to see if the string is the correct length
    if len(puzzle) != X*Y*Z:
        return 0
    
    res = puzzle_search(puzzle, X, Y, Z)
   
    if res:
        print 1 
    else:
        print 0 

    if printOn: 
        for row in res[0::2]:
            print row

def puzzle_search(puzzleString, X, Y, Z):
    """
    Returns a list of states (specified as (x, y, z, orientation) coordinates and actions
    actions are:
    none: if the junction is a straight junction
    forward: change to a forward orientation
    back: change to a backward orientation
    up: change to an up orientation
    down: change to a down orientation
    right: change to a right orientation
    left: change to a left orientation
    """
    
    puzzle = [int(item) for item in list(puzzleString)]

    ## start point, assumed to start at location (1,1,1) with right orientation
    frontier = [[(1,1,1,'right')]]
    explored = set([])

    fail = []

    ## make an array with the cubelets found in a successful cube
    goalCube = [(x,y,z) for x in xrange(1,X+1) for y in xrange(1,Y+1) for z in xrange(1,Z+1)]

    while frontier:
        path = frontier.pop(0)
        state = path[-1]
        ## get the type of junction, straight or a turn
        junctionType = puzzle[len(path[0::2])-1]

        ## iterate through each successor state
        for action, state in successors(state, junctionType, path, X, Y, Z).items():
            path2 = path + [action, state]
            if tuple(path2) not in explored:
                explored.add(tuple(path2))
                if isGoal(path2, goalCube):
                    return path2
                else:
                    frontier.append(path2)
    return fail

def successors(state, jType, path, X, Y, Z):
    
    ## create a dictionary of the possible moves for each orientation
    sides = dict([('forward',('right','left','up','down')),
                  ('back',('right','left','up','down')),
                  ('right',('up','down','forward','back')),
                  ('left',('up','down','forward','back')),
                  ('up',('forward','back','right','left')),
                  ('down',('forward','back','right','left'))])

    ## create a dictionary of the location deltas for each move
    deltas = dict([('up',(-1,0,0)),
                   ('down',(1,0,0)),
                   ('forward',(0,0,-1)),
                   ('back',(0,0,1)),
                   ('right',(0,1,0)),
                   ('left',(0,-1,0))])

    ## strip out the orientation and the actions
    stripPath = [(x,y,z) for x,y,z,_ in path[0::2]] 
    
    output = {}

    ## unpack the current state
    row, col, level, orient = state

    ## if the junction is straight, keep the current orientation and update the location
    if jType == 0:
        newRow, newCol, newLevel = row + (deltas[orient])[0], col + (deltas[orient])[1], level + (deltas[orient])[2]
        ## check to see if a cubelet already occupies the new position and if the location is within the cube boundries
        if (newRow, newCol, newLevel) not in stripPath and newRow > 0 and newRow <= X and newCol > 0 and newCol <= Y  and newLevel > 0 and newLevel <= Z:  
            output['none'] = (newRow, newCol, newLevel, orient)
        return output

    ## otherwise create a dictionary with the potential successor states. 
    else:

        ## iterate through each possible movement for the given orientation. 
        ## attachments that would lead to a cubelet occupying an already occupied location are
        ## not allowed

        for side in sides[orient]:
            newRow, newCol, newLevel = row + (deltas[side])[0], col + (deltas[side])[1], level + (deltas[side])[2]
          ## check to see if a cubelet already occupies the new position and if the location is within the cube boundries
            if (newRow, newCol, newLevel) not in stripPath and newRow > 0 and newRow <= X and newCol > 0 and newCol <= Y  and newLevel > 0 and newLevel <= Z:
                output[side] = (newRow, newCol, newLevel, side)

    return output
        
                
def isGoal(path, gCube):
    """
    Returns True if the path parameter contains the cubelets
    specified in the gCube parameter, False otherwise.

    Takes a list representing the states, actions and a list of the
    cubelets contained in a valid solution cube
    """
    
    ## strip out the orientation and the actions
    testPath = [(x,y,z) for x,y,z,_ in path[0::2]]

    ## check if each cubelet is in the list
    for cubelet in gCube:
        if cubelet not in testPath:
            return False
    return True

def test2():

    string1 = '001110111011111111011110101'
    
    snake_cube_solution(3,3,3, string1,True)


def test():

    string  = '001110110111010111101010100'
    string1 = '001110110111010111101010101'
    string2 = '001011011011110101011010101' 
    string3 = '001110111011111111011110101' 
    string4 = '011110101111111111101011111' 
    string5 = '001111111110111111101011111'
    string6 = '11111111'
    string7 = '00000000'

    stringf  = '001110110111010111101010000'
    string1f = '001110110111010111101010001'
    string2f = '001011011011110101011000001' 
    string3f = '001110111011111000011110101' 
    string4f = '011110101111111100001011111' 
    string5f = '001111111110111100001011111'
    
    
    snake_cube_solution(3,3,3, string)
    snake_cube_solution(3,3,3, string1)
    snake_cube_solution(3,3,3, string2)
    snake_cube_solution(3,3,3, string3)
    snake_cube_solution(3,3,3, string4)
    snake_cube_solution(3,3,3,string5)
    snake_cube_solution(2,2,2, string6)
    snake_cube_solution(2,2,2, string7)

    snake_cube_solution(3,3,3, stringf)
    snake_cube_solution(3,3,3, string1f)
    snake_cube_solution(3,3,3, string2f)
    snake_cube_solution(3,3,3, string3f)
    snake_cube_solution(3,3,3, string4f)
    snake_cube_solution(3,3,3,string5f)
    





