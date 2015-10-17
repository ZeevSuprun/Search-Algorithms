//
//  main.cpp
//  MazeSolver
//
//  Created by Zeev Suprun on 2014-06-25.
//  Copyright (c) 2014 Zeev Suprun. All rights reserved.
//

#include <iostream>
#include <fstream>

int solve(char** , int, int);

struct tuple {
    int x;  //x coordinate
    int y;  //y coordinate.
    int n;  //the number of steps to get to that position from the start.
};

struct tupleList {
    
    int listLen;
    int maxLen;
    
    struct tuple *list;
    
    bool inList(int xx, int yy) {
        for (int i = 0; i < listLen; i++) {
            if (list[i].x == xx && list[i].y == yy) {
                return true;
            }
        }
        return false;
    }
    int addTuple(int xx, int yy, int nn) {
        if (listLen == maxLen) {
            return -1;
        }
        list[listLen].x = xx;
        list[listLen].y = yy;
        list[listLen].n = nn;
        listLen += 1;
        return 0;
    }
    
    tupleList(int len = 0) {
        maxLen = len;
        listLen = 0;
        list = new tuple [maxLen];
        
    }
};


int main(int argc, const char * argv[]) {
    
    std::ifstream mazeData;
    std::ofstream outputFile;
    mazeData.open("mazes.txt");
    outputFile.open("output.txt");
    int numMazes, numRows, numCols;
    mazeData >> numMazes;
    
    std::cout << numMazes << "\n";
    
    char** maze;
    for (int i = 0; i < numMazes; i++) {
        
        mazeData >> numRows;
        mazeData >> numCols;
        std::cout << numRows << " " << numCols << "\n";

        maze = new char* [numRows];
        
        for (int j = 0; j < numRows; j++) {
            maze[j] = new char[numCols];
            for (int k = 0; k < numCols; k++) {
                mazeData >> maze[j][k];
            }
        }
        
        //The maze array is now fully populated.
        int minSteps = solve(maze,numRows,numCols);
        outputFile << "You can reach the end of maze " << i + 1 << " in " << minSteps << " steps\n";

    }
    
    
    return 0;
}




int solve(char** maze, int numRows, int numCols) {

    //There's a list of visited coordinates.
    //There a list of given coordinates.
    //The given coordinates are all coordinates you can get to in n steps.
    //Make a new list of positions adjecent to the given positions.
    //Append the old given positions to the list of given positions
    //increment n.
    //The new list is now the new list of given positions.
    tupleList positions (numRows * numCols);
    positions.addTuple(0, 0, 0); //Adds the tuple (0,0) to the list of positions visited.
    
    int stepCounter = 1;
    int xPos, yPos;
    bool canGoRight, canGoLeft, canGoUp, canGoDown;
    bool winRight, winLeft, winUp, winDown;
    int newPositions = 0;
    
    while (positions.listLen != positions.maxLen) {
        newPositions = 0;
        for (int i = 0; i < positions.listLen; i++) {
            xPos = (positions.list[i]).x;
            yPos = (positions.list[i]).y;
        
            //These variables are true if it's possible to go to the corresponding direction.
            canGoRight = !((xPos + 1 >= numCols) || maze[yPos][xPos + 1] == 'X' || positions.inList(xPos + 1, yPos));
            canGoLeft = !((xPos - 1 < 0) || maze[yPos][xPos - 1] == 'X' || positions.inList(xPos - 1, yPos));
            canGoUp = !((yPos + 1 >= numRows) || maze[yPos + 1][xPos] == 'X' || positions.inList(xPos, yPos + 1));
            canGoDown = !((yPos - 1 < 0) || maze[yPos - 1][xPos] == 'X' || positions.inList(xPos, yPos - 1));
            
            //These variables are true if you win by going in the corresponding direction. They exist only for readability.
            winRight = canGoRight && (maze[yPos][xPos + 1] == 'F');
            winLeft = canGoLeft && maze[yPos][xPos - 1] == 'F';
            winUp = canGoUp && maze[yPos + 1][xPos] == 'F';
            winDown = canGoDown && maze[yPos - 1][xPos] == 'F';
            if (winRight || winLeft || winUp || winDown) {
                //If one of the adjacent tiles is the end point, return
                //the number of hexes to get to the end point.
                return positions.list[i].n + 1;
            }
            
            //If you can go to a place, add that place to the list of positions.
            if (canGoRight) {
                positions.addTuple(xPos+1, yPos, positions.list[i].n + 1);
            }
            if (canGoLeft) {
                positions.addTuple(xPos-1, yPos, positions.list[i].n + 1);
            }
            if (canGoUp) {
                positions.addTuple(xPos, yPos+1, positions.list[i].n + 1);
            }
            if (canGoDown) {
                positions.addTuple(xPos, yPos-1, positions.list[i].n + 1);
            }
            
            newPositions += canGoRight + canGoLeft + canGoUp + canGoDown;
        }
        //After the for loop, we have iterated through all the positions in the list
        //and added all the places you can reach in stepcounter steps.
        //It is now time to increment stepcounter and add more positions.
        stepCounter +=1;
        if (newPositions == 0) {
            //This means there were no new places to go, which means we've hit a dead end.
            //Since the function hasn't returned, the maze is unsolvab.e
            return -1;
        }
    }
    return 0;
}