from node import *
import maze as mz
import score
import student

import numpy as np
import pandas
import time
import sys
import os

def main():
    maze = mz.Maze("data/maze_405_20180502.csv")
    next_nd = maze.getStartPoint()#node[0]--->node number 1
    car_dir = Direction.SOUTH
    point = score.Scoreboard("data/UID.csv")
    interface = student.interface()         #the part of calling student.py was commented out.

    if(1):# get a command '0',sys.argv[1] == '0'
        dirlist = []
        roadlist = []
        while (1):

            #TODO: Impliment your algorithm here and return the UID for evaluation function
            ndList = maze.strategy(next_nd)
            if ndList == []:
                break
            print "ndList:",ndList
            for i in range(len(ndList)-1):
                #interface.send_action(maze.getAction(car_dir,maze.nd_dict[ndList[i]-1],maze.nd_dict[ndList[i+1]-1]))
                dirlist.append(int(maze.getAction(car_dir,maze.nd_dict[ndList[i]-1],maze.nd_dict[ndList[i+1]-1])))
                car_dir = maze.nd_dict[ndList[i]-1].getDirection(maze.nd_dict[ndList[i+1]-1])
                roadlist.append(car_dir)
            next_nd = ndList[-1]
            maze.explored.add(next_nd)
          
            # ================================================
            # Basically, you will get a list of nodes and corresponding UID strings after the end of algorithm.
			# The function add_UID() would convert the UID string score and add it to the total score.
			# In the sample code, we call this function after getting the returned list. 
            # You may place it to other places, just make sure that all the UID strings you get would be converted.
            # ================================================
        
        dirlist.append(5)
        print "roadlist:",roadlist
        print "dirlist:",dirlist
        
        steps = 0
        
        while (1):
            if steps <= len(dirlist):
                get_UID = interface.wait_for_node();
                if get_UID == 'g':
                    interface.send_action(dirlist[steps])
                    print "next action:",dirlist[steps]
                    steps += 1
                elif len(get_UID) > 1:
                    print "UID =",get_UID
                    point.add_UID(get_UID)
                    interface.send_action(dirlist[steps])
                    print "next action:",dirlist[steps]
                    steps += 1 
            elif steps > len(dirlist):
                interface.end_process()
                break

        '''for i in range(1,len(dirlist)):
            while interface.wait_for_node()
                interface.send_action(dirlist[i])'''
    
    elif(sys.argv[1] == '1'):# get a command '1'

        while (1):

            #TODO: Implement your algorithm here and return the UID for evaluation function
            nd = int(input("destination: "))
            
            dirlist = []
            roadlist = []
            
            if(nd == 0):
            	print("end process")
            	print('')
                interface.end_process()
            	break
            
            ndList = maze.stategy_2(next_nd,nd)[0]
            print "ndList:",ndList
            
            for i in range(len(ndList)-1):
                dirlist.append(int(maze.getAction(car_dir,maze.nd_dict[ndList[i]-1],maze.nd_dict[ndList[i+1]-1])))
                car_dir = maze.nd_dict[ndList[i]-1].getDirection(maze.nd_dict[ndList[i+1]-1])
                roadlist.append(car_dir)
            next_nd = ndList[-1]
            maze.explored.add(next_nd)
            
            dirlist.append(5)
            print "roadlist:",roadlist
            print "dirlist:",dirlist
          
            steps = 0

            while (1):
                if steps <= len(dirlist):
                    get_UID = interface.wait_for_node();
                    if get_UID == 'g':
                        interface.send_action(dirlist[steps])
                        print "next action:",dirlist[steps]
                        steps += 1
                    elif len(get_UID) > 1:
                        print "UID =",get_UID
                        point.add_UID(get_UID)
                        interface.send_action(dirlist[steps])
                        print "next action:",dirlist[steps]
                        steps += 1 
                elif steps > len(dirlist):
                    #interface.end_process()
                    break
    """
    node = 0
    while(not node):
        node = interface.wait_for_node()

    interface.end_process()
    """
    print("complete")
    print("")
    a = point.getCurrentScore()
    print("The total score: ", a)

if __name__=='__main__':
    main()