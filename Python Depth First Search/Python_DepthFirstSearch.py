__author__ = 'zeev'

def solveMaze(maze, numRows, numCols, pastPositions, pos = (0,0)):
    '''
    maze is a matrix containing the maze, with numRows rows and numCols columns.
    pos is a tuple holding your x and y position, defaults to (0,0). (column, row)
    pastPositions is an array of tuples, where each tuple is an x and y coordinate that has been visited before
    Given a maze, the function returns the minimum number of steps to solve the maze.
    Uses depth first search.
    '''
    pastPositions = pastPositions.union({pos})
    print(pastPositions)
    # Check every direction to see if you can go there or if you're done.

    #You can't go there if:
    # The the position lies outside the maze
    # The character is an X
    # The position is one that you've visited already.

    canGoLeft = not(((pos[0] - 1) < 0) or ((pos[0] - 1, pos[1]) in pastPositions) or maze[pos[1]][pos[0] - 1] == 'X')
    canGoRight = not(((pos[0] + 1) > numCols - 1) or ((pos[0] + 1, pos[1]) in pastPositions) or maze[pos[1]][pos[0] + 1] == 'X')
    canGoUp = not(((pos[1] - 1) < 0) or ((pos[0], pos[1] - 1) in pastPositions) or maze[pos[1] - 1][pos[0]] == 'X')
    canGoDown = not(((pos[1] + 1) > numRows - 1) or ((pos[0], pos[1] + 1) in pastPositions) or maze[pos[1] + 1][pos[0]] == 'X')

    if ((canGoLeft and maze[pos[1]][pos[0] - 1] == 'F') or (canGoRight and maze[pos[1]][pos[0] + 1] == 'F') \
            or (canGoUp and maze[pos[1] - 1][pos[0]] == 'F') or (canGoDown and maze[pos[1] + 1][pos[0]] == 'F')):
        #The number of past positions is the number of steps you need to win,
        # not including the first position but including the last position.
        #print("Reached the end in {} moves.".format(len(pastPositions)))
        return len(pastPositions)

    if (not(canGoLeft) and not(canGoRight) and not(canGoUp) and not(canGoDown)): #dead end.
        #print("Dead end.\n")
        return 0

    leftMoves, rightMoves, upMoves, downMoves = 0,0,0,0
    if (canGoLeft):
        #print("Left.")
        leftMoves = solveMaze(maze, numRows, numCols, pastPositions, (pos[0] - 1, pos[1]))
    if (canGoRight):
        #print("Right.")
        rightMoves = solveMaze(maze, numRows, numCols, pastPositions, (pos[0] + 1, pos[1]))
    if (canGoUp):
        #print("Up.")
        upMoves = solveMaze(maze, numRows, numCols, pastPositions, (pos[0], pos[1] - 1))
    if (canGoDown):
        #print("Down.")
        downMoves = solveMaze(maze, numRows, numCols, pastPositions, (pos[0], pos[1] + 1))

    possibleAnswers = []

    for j in ([leftMoves, rightMoves, upMoves, downMoves]):
        if j != 0:
            possibleAnswers.append(j)

    if (len(possibleAnswers) == 0):
        return 0
    else:
        return min(possibleAnswers)

inFile = open("mazes.txt", "r")
outFile = open("output.txt", "w")

numMazes = int(inFile.readline())

for i in range(numMazes):
    print("Maze {}:\n".format(i + 1))
    numRows = int(inFile.readline())
    numCols = int(inFile.readline())

    maze = []

    for j in range(numRows):
        maze.append(list((inFile.readline()).strip()))
    #maze is now a matrix containing the maze.
    fewestMoves = solveMaze(maze, numRows, numCols, set())

    if (fewestMoves == 0):
        outFile.write("Maze {}: Unsolvable\n".format(i + 1))
    else:
        outFile.write("Maze {}: Solved in {} moves.\n".format(i + 1, fewestMoves))

