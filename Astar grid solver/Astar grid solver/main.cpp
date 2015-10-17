//
//  main.cpp
//  Astar grid solver
//
//  Created by Zeev Suprun on 2015-02-25.
//

#include <iostream>

struct Node {
public:
    int row, col;
    //cost to get to this grid point, cost to get to the destination from this grid point.
    int G, H;
    Node *parent;
    
    Node () {
        parent = nullptr;
    }
    Node(int r, int c, int GetHereCost, int endR, int endC, Node* par) {
        row = r;
        col = c;
        G = GetHereCost;
        H = abs(endR - r) + abs(endC - c);
        parent = par;
        
    }
};

struct nodeList {
    Node list[8*7];
    //This number of elements in the list.
    int numElements;
    
    nodeList() {
        numElements = 0;
    }
    
    int inList(int r, int c) {
        //If an element with row r and column c is in the list, return its index.
        //otherwise return -1.
        for (int i = 0; i < numElements; i++){
            if(list[i].row == r and list[i].col == c) {
                return i;
            }
        }
        return -1;
    }
    
    int removeNode(int index) {
        //Remove the node in list at index.
        if (index > numElements or numElements == 0) {
            return -1;
        }
        //To remove a node, set it equal to the last node (because order doesn't matter)
        //and decrement lastIndex. (The list now technically has a duplicate
        //but the duplicate should never be accessed.)
        list[index] = list[numElements-1];
        numElements -= 1;
        
        return 0;
    }
    int addNode(Node newNode) {
        //If the node isn't already
        
        int isPresent = inList(newNode.row, newNode.col);
        
        if ( isPresent != -1) {
            //if the node is already in the list
            if (newNode.G < list[isPresent].G) {
                //If the path to get to this new node is shorter than the last recorded path
                //to get to this node, then replace the node in the list with the new node.
                list[isPresent].G = newNode.G;
                list[isPresent].parent = newNode.parent;
            }
            
            return isPresent;
            
        } else {
            //if the node isn't already in the list, add it in and increase the list size.
            list[numElements] = newNode;
            numElements += 1;
            
            return numElements - 1;
        }
    }
    
    int findLowestF() {
        int lowestF = 1000;
        int fIndex = 0;
        
        for (int i = 0; i < numElements; i++) {
            if (list[i].G + list[i].H < lowestF){
                lowestF =list[i].G + list[i].H;
                fIndex = i;
            }
        }
        
        return fIndex;
    }
    
};

//A* maze solver
nodeList solve(int startRow, int startCol, int endRow, int endCol, char arena[8][7]);

int main(int argc, const char * argv[])
{
    /*
    nodeList list1;
    list1.addNode(Node(8,0,0,5,5,nullptr));
    Node nodeynode = list1.list[0];
    nodeList list2;
    list2.addNode(nodeynode);
    Node* nodeptr = &list2.list[0];
    
    list1.removeNode(0);

    
    list1.addNode(Node(0,0,0,5,5,nullptr));
    */
    /*
     One possible obstacle configuration:
     
     0000000
     0000xx0
     0000xx0
     00xx000
     00xx000
     0000000
     0000000
     x00000x
     top left corner is (0,0)
     using (row, column), not (x,y)
     */
    char arena[8][7];
    //populating the arena
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 7; j++) {
            arena[i][j] = '0';
        }
    }
    arena[7][0] = 'x';
    arena[7][6] = 'x';
    arena[4][2] = 'x';
    arena[4][3] = 'x';
    arena[3][2] = 'x';
    arena[3][3] = 'x';
    arena[1][4] = 'x';
    arena[1][5] = 'x';
    arena[2][4] = 'x';
    arena[2][5] = 'x';
    
    nodeList path = solve(7, 3, 0, 0, arena);
    
    int r, c;
    for (int i = 0; i < path.numElements; i++) {
        r = path.list[i].row;
        c = path.list[i].col;
        std::cout << '(' << r << ',' << c << ")\n";
    }
    
    
    return 0;
}

nodeList solve(int startRow, int startCol, int endRow, int endCol, char arena[8][7]) {
    //arena is filled with '0' and 'x' for allowed and blocked spaces, respectively.
    //Arena can only be an 8*7 grid because this was the size of the grid the robot could maneuver on.
    
    //list of grid points still to check.
    nodeList openList;
    //Adding the first node to the open list.
    openList.addNode(Node(startRow, startCol, 0, endRow, endCol, nullptr));
    
    //list of grid points that have already been checked.
    nodeList closedList;
    nodeList path;
    Node nextInPath;
    
    //Check all squares adjacent to start point and add them in the open list if
    //they are valid places to go. Drop the start square into the closed list.
    
    //Choose the lowest F square in the open list, check all adjacent squares
    //and add them to the open list if they aren't there already.
    //(If they're already in the open list then update them if necessary)
    //Then drop the parent square into the closed list.
    
    //index of the node being checked in openList, index of that same node in closedList, index of the node to be returned.
    int nodeToCheckIndex, newIndex, returnIndex;
    Node toCheck;
    bool canGoRight, canGoLeft, canGoUp, canGoDown;
    
    while (true) {
        
        //Find the node with the lowest F.
        nodeToCheckIndex =openList.findLowestF();
        toCheck = openList.list[nodeToCheckIndex];
        
        //remove toCheck from the openList and add it to closedList
        newIndex = closedList.addNode(toCheck);
        openList.removeNode(nodeToCheckIndex);
        
        //Check to see which nodes are open from this node.
        
        //you can go right if you're not at the edge and that position isn't blocked.
        canGoRight = (toCheck.col + 1 < 7) and arena[toCheck.row][toCheck.col + 1] != 'x';
        canGoLeft = (toCheck.col - 1 >= 0) and arena[toCheck.row][toCheck.col - 1] != 'x';
        canGoUp = (toCheck.row - 1 >= 0) and arena[toCheck.row - 1][toCheck.col] != 'x';
        canGoDown = (toCheck.row + 1 < 8) and arena[toCheck.row + 1][toCheck.col] != 'x';
        
        //These 4 if statements add the 4 directions to the open list if they are valid points.
        //They also check to see if any of the new grid points added are the destination.
        if (canGoRight) {
            returnIndex = openList.addNode(Node(toCheck.row, toCheck.col + 1, toCheck.G + 1, \
                                                endRow, endCol, &closedList.list[newIndex]));
            if(toCheck.row == endRow and toCheck.col == endCol) {
                //if this node is the end node, travel up the path to return it.
                nextInPath = openList.list[returnIndex];
                while(true) {
                    path.addNode(nextInPath);
                    if (nextInPath.parent == nullptr) {
                        break;
                    } else {
                        nextInPath = *(nextInPath.parent);
                    }
                }
                return path;
            }
        }
        if (canGoLeft) {
            returnIndex = openList.addNode(Node(toCheck.row, toCheck.col - 1, toCheck.G + 1, \
                                                endRow, endCol, &closedList.list[newIndex]));
            if(toCheck.row == endRow and toCheck.col == endCol) {
                //if this node is the end node, travel up the path to return it.
                nextInPath = openList.list[returnIndex];
                while(true) {
                    path.addNode(nextInPath);
                    if (nextInPath.parent == nullptr) {
                        break;
                    } else {
                        nextInPath = *(nextInPath.parent);
                    }
                }
                return path;
            }
        }
        if (canGoUp) {
            returnIndex = openList.addNode(Node(toCheck.row-1, toCheck.col, toCheck.G + 1, \
                                                endRow, endCol, &closedList.list[newIndex]));
            if(toCheck.row == endRow and toCheck.col == endCol) {
                //if this node is the end node, travel up the path to return it.
                nextInPath = openList.list[returnIndex];
                while(true) {
                    path.addNode(nextInPath);
                    if (nextInPath.parent == nullptr) {
                        break;
                    } else {
                        nextInPath = *(nextInPath.parent);
                    }
                }
                return path;
            }
        }
        if (canGoDown) {
            returnIndex = openList.addNode(Node(toCheck.row+1, toCheck.col, toCheck.G + 1, \
                                                endRow, endCol, &closedList.list[newIndex]));
            if(toCheck.row == endRow and toCheck.col == endCol) {
                //if this node is the end node, travel up the path to return it.
                nextInPath = openList.list[returnIndex];
                while(true) {
                    path.addNode(nextInPath);
                    if (nextInPath.parent == nullptr) {
                        break;
                    } else {
                        nextInPath = *(nextInPath.parent);
                    }
                }
                return path;
            }
        }
    }
}