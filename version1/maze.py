from node import *
import numpy as np
import csv
import pandas
from enum import IntEnum
from Queue import *

class Action(IntEnum):
    ADVANCE    = 1
    U_TURN     = 2
    TURN_RIGHT = 3
    TURN_LEFT  = 4
    HALT       = 5

class Maze:
    def __init__(self, filepath):
        self.raw_data = pandas.read_csv(filepath).values
        self.nodes = []
        self.nd_dict = dict() # key: index, value: the correspond node
        self.explored = set()

        for dt in self.raw_data:
            #TODO: Update the nodes with the information from raw_data
            self.nodes.append(Node(dt[0]))
            self.nd_dict[dt[0]-1] = self.nodes[-1]

        for i in range(len(self.nodes)):
            for j in range(1,5):
                #TODO: Update the successors for each node
                if self.raw_data[i][j] > 0:
                    self.nodes[i].setSuccessor(self.nodes[int(self.raw_data[i][j])-1].getIndex(),j,self.raw_data[i][j+4])

    def getStartPoint(self):

        if (len(self.nd_dict) < 2):
            print ("Error: the start point is not included.")
            return 0;
        return self.nd_dict[0].getIndex()
    
    def BFS(self, nd):
        """ return a sequence of nodes from the node to the nearest unexplored deadend"""
        #TODO: design your data structure here for your algorithm
        u = 0
        roads = dict()# key: length, value: path[]
        roadslength = []# all length of each path are in here
        prevNode = 0 
        start = 0
        
        for deadend in range(len(self.nodes)):
            if deadend != nd-1 and self.nd_dict[deadend].isEnd() and deadend+1 not in self.explored:
                path = self.BFS_2(nd,deadend+1)# a tople,([path],length)
                roads[path[1]] = path[0]
                roadslength.append(path[1])
        #TODO: update the information of your data structure
        if len(roadslength):
            shortestlength = min(roadslength)
            return roads[shortestlength]
        else:
            return []

    def BFS_2(self, nd_from, nd_to):
        """ return a sequence of nodes of the shortest path"""
        #TODO: similar to BFS but fixed start point and end point
        ndList = []
        
        distances_dict = {int(node)+1 : 0 for node in self.nd_dict}# key: node(index), value: distance from start (initialize to 0)
        distances_dict[len(self.nd_dict)] = 0
        nowDistance = 0
        
        u = 0
        prevNode = 0 
        start = 0
        pathlength = 0

        if nd_to != nd_from:
            start = nd_from
            q = Queue()
            q.put(nd_to)
                
            transitionTable = dict()
            explored = []
                
            while (True):
                u = q.get()
                
                if u not in explored:
                    explored.append(u)
                
                if len(explored) == len(self.nd_dict):
                    # all nodes are explored
                    break
                
                for v in self.nd_dict[u-1].getSuccessors():
                    nowDistance = self.nd_dict[u-1].getLength(self.nd_dict[v[0]-1])
                    newDistance = distances_dict[u] + nowDistance
                    
                    if v[0] not in explored:# same node can come from different node,but all possibility will send to queue
                        q.put(v[0])
                    
                    if distances_dict[v[0]] == 0 or distances_dict[v[0]] > newDistance:# only when the distance is shorter than before that the transitiontable being renewed
                        distances_dict[v[0]] = newDistance
                        transitionTable[v[0]] = u
            #TODO: Apply your algorithm here. Make sure your algorithm can update values and stop under some conditions.
            ndList.append(start)
            while start != nd_to:
                prevNode = transitionTable[start]
                ndList.append(prevNode)
                pathlength += self.nd_dict[start-1].getLength(self.nd_dict[prevNode-1])
                start = prevNode
        #TODO: update the information of your data structure 
        return ndList,pathlength

    def getAction(self, car_dir, nd_from, nd_to):
        """ return an action and the next direction of the car """
        if nd_from.isSuccessor(nd_to):
            nd_dir = nd_from.getDirection(nd_to)
            if car_dir == Direction.NORTH:
                if nd_dir == Direction.NORTH:
                    return Action.ADVANCE
                elif nd_dir == Direction.SOUTH:
                    return Action.U_TURN
                elif nd_dir == Direction.WEST:
                    return Action.TURN_LEFT
                else:
                    return Action.TURN_RIGHT
            elif car_dir == Direction.SOUTH:
                if nd_dir == Direction.SOUTH:
                    return Action.ADVANCE
                elif nd_dir == Direction.NORTH:
                    return Action.U_TURN
                elif nd_dir == Direction.EAST:
                    return Action.TURN_LEFT
                else:
                    return Action.TURN_RIGHT
            elif car_dir == Direction.WEST:
                if nd_dir == Direction.WEST:
                    return Action.ADVANCE
                elif nd_dir == Direction.EAST:
                    return Action.U_TURN
                elif nd_dir == Direction.SOUTH:
                    return Action.TURN_LEFT
                else:
                    return Action.TURN_RIGHT
            else:
                if nd_dir == Direction.EAST:
                    return Action.ADVANCE
                elif nd_dir == Direction.WEST:
                    return Action.U_TURN
                elif nd_dir == Direction.NORTH:
                    return Action.TURN_LEFT
                else:
                    return Action.TURN_RIGHT
            #TODO: Return the action based on the current car direction and the direction to next node
            print("Error: Failed to get the action")
            return 0
        else:
            print("Error: Node(",nd_to.getIndex(),") is not the Successor of Node(",nd_from.getIndex(),")")
            return 0

    def strategy(self, nd):
        return self.BFS(nd)

    def strategy_2(self, nd_from, nd_to):
        return self.BFS_2(nd_from, nd_to)