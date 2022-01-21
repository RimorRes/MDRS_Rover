import numpy as np


class Node:

    def __init__(self, parent=None, position=None):
        self.parent = parent
        self.position = position
        self.g = 0
        self.h = 0
        self.f = 0

    def __eq__(self, other):
        return np.array_equal(self.position, other.position)


def a_star(graph, start, goal):

    graph_h, graph_w = graph.shape

    start_node = Node(position=start)
    start_node.f = 0
    goal_node = Node(position=goal)

    open_list = [start_node]
    closed_list = []
    analyzed = 0
    while open_list:

        open_list.sort(key=lambda n: n.f)
        q = open_list.pop(0)
        analyzed += 1

        children = []
        x, y = q.position

        for horz in range(-1, 2):
            for vert in range(-1, 2):

                if not (horz == 0 and vert == 0):

                    if (0 <= x+horz < graph_w) and (0 <= y+vert < graph_h):

                        if graph[y+vert, x+horz] == 0:  # traversable node

                            c = Node(
                                parent=q,
                                position=np.array([x+horz, y+vert])
                            )
                            children.append(c)

        for child in children:

            if child == goal_node:
                path = []
                node = child
                while not node == start_node:
                    path.append(node.position)
                    node = node.parent
                path.append(node.position)
                path.reverse()

                return path, analyzed

            child.g = q.g + 1
            child.h = np.linalg.norm(goal_node.position - child.position)
            child.f = child.g + child.h

            if child not in closed_list:
                if child in open_list:
                    ind = open_list.index(child)
                    curr = open_list[ind]
                    if child.f < curr.f:
                        del open_list[ind]
                        open_list.append(child)
                else:
                    open_list.append(child)

        closed_list.append(q)

    raise Exception('Goal was never reached')
