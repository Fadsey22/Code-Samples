import sys

from problem_spec import ProblemSpec
from robot_config import write_robot_config_list_to_file
from robot_config import make_robot_config_from_ee1
from robot_config import  make_robot_config_from_ee2
from robot_config import  RobotConfig
from tester import test_config_equality
import random
from tester import test_obstacle_collision
from tester import  test_self_collision
from tester import test_environment_bounds
from tester import  test_angle_constraints
from tester import test_length_constraints
from angle import  Angle
import numpy as np
import time

"""
Template file for you to implement your solution to Assignment 2. Contains a class you can use to represent graph nodes,
and a method for finding a path in a graph made up of GraphNode objects.

COMP3702 2020 Assignment 2 Support Code
"""


class GraphNode:
    """
    Class representing a node in the state graph. You should create an instance of this class each time you generate
    a sample.
    """

    def __init__(self, spec, config):
        """
        Create a new graph node object for the given config.

        Neighbors should be added by appending to self.neighbors after creating each new GraphNode.

        :param spec: ProblemSpec object
        :param config: the RobotConfig object to be stored in this node
        """
        self.spec = spec
        self.config = config
        self.neighbors = []

    def __eq__(self, other):
        return test_config_equality(self.config, other.config, self.spec)

    def __hash__(self):
        return hash(tuple(self.config.points))

    def get_successors(self):
        return self.neighbors







    @staticmethod
    def add_connection(n1, n2):
        """
        Creates a neighbor connection between the 2 given GraphNode objects.

        :param n1: a GraphNode object
        :param n2: a GraphNode object
        """
        n1.neighbors.append(n2)
        n2.neighbors.append(n1)


def find_graph_path(spec, init_node):
    """
    This method performs a breadth first search of the state graph and return a list of configs which form a path
    through the state graph between the initial and the goal. Note that this path will not satisfy the primitive step
    requirement - you will need to interpolate between the configs in the returned list.

    You may use this method in your solver if you wish, or can implement your own graph search algorithm to improve
    performance.

    :param spec: ProblemSpec object
    :param init_node: GraphNode object for the initial configuration
    :return: List of configs forming a path through the graph from initial to goal
    """
    # search the graph
    init_container = [init_node]

    # here, each key is a graph node, each value is the list of configs visited on the path to the graph node
    init_visited = {init_node: [init_node.config]}

    while len(init_container) > 0:
        current = init_container.pop(0)

        if test_config_equality(current.config, spec.goal, spec):
            # found path to goal
            return init_visited[current]

        successors = current.get_successors()
        for suc in successors:
            if suc not in init_visited:
                init_container.append(suc)
                init_visited[suc] = init_visited[current] + [suc.config]

    return None


def generate_robot(robot, spec):
    randx = round(random.uniform(0.0, 1.0), 1)
    randy = round(random.uniform(0.0, 1.0), 1)
    angles = []
    lengths = []
    i = 0
    while i < len(robot.lengths):
        angles.append(random.randrange(-165, 165, 1))
        lengths.append(random.uniform(spec.min_lengths[i], spec.max_lengths[i]))
        i += 1

    config = make_robot_config_from_ee1(robot.get_ee1()[0], robot.get_ee1()[1], angles, lengths, ee1_grappled=True)
    return config

def generate_robot1(robot, spec):
    lengths = []
    angles = []
    i = 0
    while i < len(robot.lengths):
        lengths.append(random.uniform(spec.min_lengths[i], spec.max_lengths[i]))
        i += 1
    angles.append(Angle(degrees=random.uniform(0, 180)))
    j = 0
    while j < len(robot.lengths) - 1:
        angles.append(Angle(degrees=random.uniform(-165, 165)))
        j += 1
    if robot.ee1_grappled:
        config = make_robot_config_from_ee1(robot.get_ee1()[0], robot.get_ee1()[1], angles, lengths, ee1_grappled=True)
    else:
        config = make_robot_config_from_ee2(robot.get_ee2()[0], robot.get_ee2()[1], angles, lengths, ee2_grappled=True)

    return config


def set_neighbors(Nodes):
    j = 0
    for node in Nodes:
        while j < len(Nodes):
            b = 0
            if node != Nodes[j] and node not in Nodes[j].neighbors:
                for a in node.config.ee1_angles:
                    if abs(Nodes[j].config.ee1_angles[b] - a) <= 10:
                        node.add_connection(node, Nodes[j])
                    b += 1
            j += 1


def interpolate(config1, config2, spec):
    angle = Angle(0.001, None)
    step = angle.in_degrees()
    x1 = config1.get_ee2()[0]
    y1 = config1.get_ee2()[1]
    x2 = config2.get_ee2()[0]
    y2 = config2.get_ee2()[1]
    angles = []
    configurations = []
    j = 0
    con = make_robot_config_from_ee1(config1.get_ee1()[0], config1.get_ee1()[1], config1.ee1_angles, config1.lengths,
                                     ee1_grappled=True, ee2_grappled=False)

    print(config2.ee1_angles[2])
    print(config1.ee1_angles[2])

    while j < len(config1.ee1_angles):
        angles.append(config1.ee1_angles[j])
        j += 1
    found = False
    while not found:
        ang = 0
        while ang < len(config1.ee1_angles):
            if angles[ang] == config2.ee2_angles[ang]:
                ang += 1
                continue
            print(angles[ang])
            angles[ang] += step
            ang += 1
        part = make_robot_config_from_ee1(config1.get_ee1()[0], config1.get_ee1()[1], angles, config1.lengths,
                                          ee1_grappled=True, ee2_grappled=False)
        if part.get_ee2()[0] == x2 and part.get_ee2()[1] == y2 and part.ee1_angles == config2.ee1_angles:
            break
        configurations.append(part)
    return configurations


def check_for_collision(config, spec):
    if not test_obstacle_collision(config,spec,spec.obstacles):
        #print("to")
        return True
    if not test_self_collision(config,spec):
       # print("ts")
        return True
    if not test_environment_bounds(config):
       # print("te")
        return True
    if not test_angle_constraints(config,spec):
      #  print("ta")
        return True
    if not test_length_constraints(config,spec):
      #  print("tl")
        return True
    return False


def interpolate1(config1, config2, spec):
    numjoins = len(config1.lengths)
    i = 0
    angle = Angle(0.001)
    step = Angle(None, 0.05729577951308232)
    diff = []
    Configurations = []
    angle_diff = []
    while i < numjoins:
        c1 = config1.ee1_angles[i]
        c2 = config2.ee1_angles[i]
        diff.append([c1, c2])
        angle_diff.append(abs(c1.in_degrees() - c2.in_degrees()))
        i += 1

    max_ang = max(angle_diff)
    steps = max_ang/step.in_degrees()
    j = 0
    while j < steps:
        angles = []
        for ang in diff:
            if(ang[0] > ang[1] - step) and (ang[0] < ang[1] + step):
                angle = ang[1]
            else:
                if ang[0] < ang[1]:
                    angle = ang[0] + step
                elif ang[0] > ang[1]:
                    angle = ang[0] - step
            ang[0] = angle
            angles.append(angle)
        con = make_robot_config_from_ee1(config1.get_ee1()[0], config1.get_ee1()[1],
                                         angles, config1.lengths, ee1_grappled=True)
        if not check_for_collision(con, spec):
            Configurations.append(con)
        else:
            return False
        j += 1
    Configurations.append(config2)
    return Configurations







def connect(n1, n2, spec):
    if not interpolate1(n1.config, n2.config, spec):
        return False
    else:
        n1.add_connection(n1, n2)
        return True





def main(arglist):
    start = time.time()
    input_file = arglist[0]
    output_file = arglist[1]
    spec = ProblemSpec(input_file)
    init_node = GraphNode(spec, spec.initial)
    goal_node = GraphNode(spec, spec.goal)
    steps = []
    steps.append(spec.initial)
    nodes = [init_node, goal_node]
    i = 0
    j = 0
    while True:
        config = generate_robot1(spec.initial, spec)
        if not check_for_collision(config, spec):
            i += 1
            node = GraphNode(spec, config)
            if node not in nodes:
                for n in nodes:
                    connect(node, n, spec)
                nodes.append(node)
        if i > 10:
            graph = find_graph_path(spec, init_node)
            if graph:
                break
            else:
                i = 0
        i += 1
    for g in graph:
        if j + 1 == len(graph):
            break
        else:
            path = interpolate1(g, graph[j+1], spec)
            for n in path:
                steps.append(n)
        j += 1
   # print(time.time() - start)















    #
    #
    # Code for your main method can go here.
    #
    # Your code should find a sequence of RobotConfig objects such that all configurations are collision free, the
    # distance between 2 successive configurations is less than 1 primitive step, the first configuration is the initial
    # state and the last configuration is the goal state.
    #
    #

    if len(arglist) > 1:
        write_robot_config_list_to_file(output_file, steps)

    #
    # You may uncomment this line to launch visualiser once a solution has been found. This may be useful for debugging.
    # *** Make sure this line is commented out when you submit to Gradescope ***
    #
    #v = Visualiser(spec, steps)


if __name__ == '__main__':
    main(sys.argv[1:])

STUDENT
Fardeen Bhuiyan
AUTOGRADER SCORE
40.0 / 60.0
