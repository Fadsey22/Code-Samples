#!/usr/bin/python
import sys
import copy
from laser_tank import LaserTankMap
import heapq
import time
"""
Template file for you to implement your solution to Assignment 1.

COMP3702 2020 Assignment 1 Support Code
"""

class Node:
    def __init__(self):
        self.position = []
        self.fscore = 0
        self.gscore = 0
        self.hscore = 0
        self.parent = None
        self.heading = None
        self.shot = False
        self.map = None
        self.move = None
        self.goal = False
        self.success = False

    def find_goal(self,map):
        coords = [0, 0]
        i = 0
        j = 0
        while map.y_size - 1 > i:
            while map.x_size - 1 > j:
                if map.grid_data[i][j] == map.FLAG_SYMBOL:
                    coords[0] = i
                    coords[1] = j
                j += 1
            i += 1
            j = 0
        return coords

    def find_distance_to_goal(self, map, x, y, ):
        goal = find_goal(map)
        distance = abs(x - goal[1]) + abs(y - goal[0])
        return distance

    def check_for_change(self, map1, map2):
        if map1 != map2:
            return True
        return False
    """
    def check_for_change(self, map1, map2):
        i = 0
        j = 0
        for i in range(map1.y_size - 1):
            for j in range(map1.x_size - 1):
        #while i < map1.y_size:
            #while j < map1.x_size:
                if not (map1.grid_data[i][j] == map2.grid_data[i][j]):
                    return True
                j += 1
            i += 1
            j = 0
        return False
        """

    def Laser_Tank_Copy(self, map1, game_map):
        map2 = LaserTankMap(game_map.x_size,game_map.y_size, game_map.grid_data,
                            game_map.player_x,game_map.player_y,game_map.player_heading)
        map2.grid_data = [row[:] for row in map1.grid_data]
        map2.player_y = map1.player_y
        map2.player_x = map1.player_x
        map2.player_heading = map1.player_heading
        return map2


    def successor(self,action, game_map):
        node = Node()
        node.map = node.Laser_Tank_Copy(self.map, game_map)
        node.parent = self
        move = node.map.apply_move(action)
        if action != 's' and move == 0:
            node.heading = node.map.player_heading
            node.shot = False
            node.position = [node.map.player_y, node.map.player_x]
            node.gscore = self.gscore + 1
            node.hscore = node.find_distance_to_goal(node.map, node.position[1], node.position[0])
            node.fscore = node.gscore + node.hscore
            node.move = action
            node.success = True
        elif action == 's' and move == 0 and self.map != node.map: #node.check_for_change(node.map, self.map):
            node.heading = node.map.player_heading
            node.shot = True
            node.position = [node.map.player_y, node.map.player_x]
            node.gscore = self.gscore + 1
            node.hscore = node.find_distance_to_goal(node.map, node.position[1], node.position[0])
            node.fscore = node.gscore + node.hscore - 1
            node.move = "s"
            node.success = True
        if node.map.is_finished():
            node.goal = True
        return node

    """

    def successor(self, action, game_map):
        node = Node()
        node.map = node.Laser_Tank_Copy(self.map, game_map)
        node.parent = self
        if action == 'f':
            if node.map.apply_move(node.map.MOVE_FORWARD) == 0:
                node.heading = node.map.player_heading
                node.shot = False
                node.position = [node.map.player_y, node.map.player_x]
                node.gscore = self.gscore + 1
                node.hscore = node.find_distance_to_goal(node.map, node.position[1], node.position[0])
                node.fscore = node.gscore + node.hscore
                node.move = "f"
                node.success = True
            else:
                node.success = False
                node.move = "f"
                node.heading = self.heading
                node.position = self.position
                node.map = self.map
                node.fscore = 10000
        elif action == 's':
            if node.map.apply_move(node.map.SHOOT_LASER) == 0 and node.check_for_change(node.map, self.map):
                node.heading = node.map.player_heading
                node.shot = True
                node.position = [node.map.player_y, node.map.player_x]
                node.gscore = self.gscore + 1
                node.hscore = node.find_distance_to_goal(node.map, node.position[1], node.position[0])
                node.fscore = node.gscore + node.hscore
                node.move = "s"
                node.success = True
            else:
                node.success = False
                node.move = "s"
                node.heading = self.heading
                node.position = self.position
                node.map = self.map
                node.fscore = 10000

        elif action == 'r':
            if node.map.apply_move(node.map.TURN_RIGHT) == 0:
                node.heading = node.map.player_heading
                node.shot = False
                node.position = [node.map.player_y, node.map.player_x]
                node.gscore = self.gscore + 1
                node.hscore = node.find_distance_to_goal(node.map, node.position[1], node.position[0])
                node.fscore = node.gscore + node.hscore
                node.move = "r"
                node.success = True
            else:
                node.success = False
                node.move = "r"
                node.heading = self.heading
                node.position = self.position
                node.map = self.map
                node.fscore = 10000

        elif action == 'l':
            if node.map.apply_move(node.map.TURN_LEFT) == 0:
                node.heading = node.map.player_heading
                node.shot = False
                node.position = [node.map.player_y, node.map.player_x]
                node.gscore = self.gscore + 1
                node.hscore = node.find_distance_to_goal(node.map, node.position[1], node.position[0])
                node.fscore = node.gscore + node.hscore
                node.move = "l"
                node.success = True
            else:
                node.success = False
                node.move = "l"
                node.heading = self.heading
                node.position = self.position
                node.map = self.map
                node.fscore = 10000
        if node.map.is_finished():
            node.goal = True
        return node
    """

    """
    def successor(self, action, input_file):
        node = Node()
        node.map = node.Laser_Tank_Copy(self.map, input_file)
        move = node.map.apply_move(action)
        if move == 0:
            node.position = [node.map.player_y, node.map.player_x]
            node.gscore += 1
            node.hscore = node.find_distance_to_goal(node.map, node.position[1], node.position[0])
            node.fscore = node.gscore + node.hscore
            node.heading = node.map.player_heading
            node.parent = self
            node.move = move
            node.success = True
        if action == 's':
            node.shot = True
        if action != 's':
            node.shot = False
        return node
        """




    def __lt__(self, other):
        return self.fscore < other.fscore

    def __eq__(self, other):
        return self.position == other.position and self.heading == other.heading and \
            self.shot == other.shot





#
#
# Code for any classes or functions you need can go here.
#
#

def write_output_file(filename, actions):
    """
    Write a list of actions to an output file. You should use this method to write your output file.
    :param filename: name of output file
    :param actions: list of actions where is action is in LaserTankMap.MOVES
    """
    f = open(filename, 'w')
    for i in range(len(actions)):
        f.write(str(actions[i]))
        if i < len(actions) - 1:
            f.write(',')
    f.write('\n')
    f.close()


def find_goal(map):
    coords = [0, 0]
    i = 0
    j = 0
    while map.y_size - 1 > i:
        while map.x_size - 1 > j:
            if map.grid_data[i][j] == map.FLAG_SYMBOL:
                coords[0] = i
                coords[1] = j
            j += 1
        i += 1
        j = 0
    return coords

def init_node(map):
    node = Node()
    node.position = [map.player_y, map.player_x]
    node.hscore = node.find_distance_to_goal(map, map.player_x, map.player_y)
    node.fscore = node.hscore + node.gscore
    node.heading = map.player_heading
    node.map = copy.deepcopy(map)
    return node

def check_seen(seen, child):
    i = 0
    while i < len(seen):
        if child.position == seen[i].position and \
                child.heading == seen[i].heading and \
                child.shot == seen[i].shot:
            return True
        i += 1
    return False

def equal_node(child,list1):
    for n in list1:
        if n.__eq__(child):
            if n.fscore > child.fscore:
                list1.remove(n)
                list1.append(child)
                return True
            else:
                return False




"""def equal_node(seen,child):
    i = 0
    while i < len(seen):
        if child.position == seen[i].position and \
                child.heading == seen[i].heading and \
                child.map == seen[i].map:
            if child.fscore < seen[i].fscore:
                seen.remove(seen[i])
                return True
            else:
                return False
        i += 1
"""
def actions_append(last):
    actions = [last.move]
    parent = last.parent
    while parent:
        actions.append(parent.move)
        parent = parent.parent
    actions.remove(None)
    final = actions[::-1]
    return final


def main(arglist):
    start = time.time()
    input_file = arglist[0]
    output_file = arglist[1]
    # Read the input testcase file
    game_map = LaserTankMap.process_input_file(input_file)
    node = init_node(game_map)
    l1 = [] #open nodes
    heapq.heappush(l1, node) #append the starting node
    heapq.heapify(l1) #convert to heap
    seen = []  #closed nodes
   # i = 0
    while l1:
        node = heapq.heappop(l1) #pop the smallest element based on Fvalue:
       #if i > 0:
            #print(node.fscore, node.move, node.parent.move)
        #i += 1
        seen.append(node)  # append this node to seen as it has been visited
        if node.goal: # if path has been found
            actions = actions_append(node)
            write_output_file(output_file, actions)
            print(time.time()-start)
            return
        for action in game_map.MOVES:
            child = node.successor(action, game_map)
            if not child.success or check_seen(seen, child):
                continue
            if not check_seen(l1, child):
                child.parent = node
                #if not check_seen(l1, child): # if not in seen list
                heapq.heappush(l1, child)
               # else:
                    #equal_node(child, l1) # if in seen but the fscore is now lower then replace















    # Code for your main method can go here.
    #
    # Your code should find a sequence of actions for the agent to follow to reach the goal, and store this sequence
    # in 'actions'.
    #
    #

    # Write the solution to the output file
    #write_output_file(output_file, actions)


if __name__ == '__main__':
   main(sys.argv[1:])
   #main(["testcases/t2_shortcut.txt", "out.txt"])


