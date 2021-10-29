import math
import numpy as np
from scipy.spatial.transform import Rotation


def normalize(v):
    norm = np.linalg.norm(v)
    if norm == 0:
        return v
    return v / norm


def sphere2cartesian(distance, inclination, azimuth):
    x = distance * math.sin(azimuth) * math.sin(inclination)
    y = distance * math.cos(azimuth) * math.sin(inclination)
    z = distance * math.cos(inclination)
    return np.array([x, y, z])


def slope(point1, point2):
    run = math.sqrt((point2[0] - point1[0]) ** 2 + (point2[1] - point1[1]) ** 2)
    rise = point2[2] - point1[2]
    if run != 0:
        return rise/run
    else:
        return np.inf


class Pathfinder:

    def __init__(self, target, sensor_pos, sensor_inclination, min_width, max_angle, step_size):
        self.target = target

        # Path picking constraints
        self.min_width = min_width
        self.max_angle = max_angle
        self.sensor_pos = sensor_pos  # sensor position relative to the rover
        self.sensor_inclination = sensor_inclination   # sensor tilt (on +X) relative to +Z axis
        self.step_size = step_size

        self.nodes = []  # list of past encountered nodes each node containing untested paths
        self.path_log = []  # stores all paths taken by the rover

    def point_cloud(self, scanline, rover_pos, rover_rot):
        # TODO: might need to take into account sensor cone_angle
        point_cloud = []  # list of all scanned points
        r = Rotation.from_euler('ZYX', rover_rot)  # apply rotation as Z-Y-X intrinsic Tait-Byran angles
        for data_point in scanline:
            coords = sphere2cartesian(
                data_point[1],
                self.sensor_inclination,
                data_point[0],
            )
            point_cloud.append(r.apply(coords + self.sensor_pos) + rover_pos)
        return point_cloud

    def eval_traversability(self, points, rover_pos):
        # TODO: take into account rover inclination
        potential_chains = []
        valid_chains = []
        invalid_points = [points[0], points[-1]]  # endpoints of scan are considered non-traversable
        c = []
        for i in range(1, len(points) - 1):
            # evaluating 'sideways' slope between 2 points
            # evaluating if terrain is too steep or is a wall by looking at slope between the rover head to the point
            slopes = [
                abs(slope(points[i - 1], points[i])),
                abs(slope(points[i], points[i + 1])),
                abs(slope(rover_pos, points[i])),
            ]
            if max(slopes) <= math.atan(self.max_angle):
                c.append(points[i])
            else:  # traversable chain ended, time to save it
                invalid_points.append(points[i])  # counting invalid points
                if len(c) > 1:  # don't want empty lists or single point lists
                    potential_chains.append(c)
                c = []  # empty chain
        # checking last chain
        if len(c) > 1:  # don't want empty lists or single point lists
            potential_chains.append(c)

        # culling narrow chains
        # TODO: rover might need to align itself with normal of traversable zone. Ex: tilted doors
        for c in potential_chains:
            endpoint1, endpoint2 = c[0], c[-1]
            width = math.sqrt((endpoint2[0] - endpoint1[0]) ** 2 + (endpoint2[1] - endpoint1[1]) ** 2)
            if width > self.min_width:
                valid_chains.append(c)
            else:
                invalid_points += c  # counting invalid points

        # Debug
        print('found', len(potential_chains), 'potential chains')
        print('found', len(valid_chains), 'valid chains')
        s = 0
        for c in valid_chains:
            s += len(c)
        print('Point distribution:')
        print('- valid points:', s)
        print('- invalid points:', len(invalid_points))
        print('- total / expected total:', s + len(invalid_points), '/', len(points))

        return valid_chains, invalid_points

    def available_paths(self, waypoint_step, valid_chains, invalid_points, rover_pos):
        waypoints = []
        # picking out certain points to act as waypoints
        for c in valid_chains:
            mid = len(c)//2
            for p in c[:mid:waypoint_step]:
                waypoints.append(p)
            waypoints.append(c[mid])
            for p in c[mid::waypoint_step]:
                waypoints.append(p)
        # Compute pathing to each waypoint on the XY plane
        valid_paths = []
        valid_waypoints = []
        for w_index in range(len(waypoints)):
            w = waypoints[w_index][:2]  # shearing off the Z component
            pos = rover_pos[:2]
            path = [pos]
            dist_waypoint = np.linalg.norm(w - pos)

            while dist_waypoint > self.step_size:

                heading = normalize(w - pos)
                next_pos = heading * self.step_size + pos

                # adjust next_pos to avoid obstacles/invalid points
                for point in invalid_points:
                    point = point[:2]  # shearing off the Z component
                    v = next_pos - point
                    if np.linalg.norm(v) < self.min_width / 2:
                        next_pos = point + normalize(v) * self.min_width / 2

                # checking pass to verify that all invalid points are avoided
                encroach = False
                for point in invalid_points:
                    point = point[:2]  # shearing off the Z component
                    v = next_pos - point
                    if np.linalg.norm(v) < self.min_width / 2:
                        encroach = True

                if not encroach:
                    path.append(next_pos)
                    pos = next_pos
                    dist_waypoint = np.linalg.norm(w - pos)  # update distance to waypoint
                else:
                    # print("Encroach Error: pathing is impossible!")
                    path = []
                    break

            if len(path) > 0:  # case where waypoint is reachable
                path.append(w)
                valid_paths.append(path)  # shape (2,)
                valid_waypoints.append(waypoints[w_index])  # shape (3,)

        # sorting paths based on alignment to target
        # avoid sorting if unnecessary
        if len(valid_paths) == 0:
            # print('No valid paths!')
            return []
        elif len(valid_paths) == 1:
            # print('Single path at fork')
            # print('waypoint:', valid_waypoints[0])
            return valid_paths
        else:
            alignments = []
            target_heading = normalize(self.target - rover_pos)
            for w in valid_waypoints:
                waypoint_heading = normalize(w - rover_pos)
                alignments.append(np.dot(target_heading, waypoint_heading))

            sorted_paths = [p for _, p in sorted(zip(alignments, valid_paths), key=lambda x: x[0], reverse=True)]

            # Debug
            # print('All paths at fork:')
            # for w, a in zip(valid_waypoints, alignments):
            #     print('- alignment:', a, '\twaypoint:', w)
            # print('Best waypoint:', sorted_paths[0][-1])

            return sorted_paths  # NOTE: only XY points!

    def evaluate_terrain(self, scanline, waypoint_step, rover_pos, rover_rot):
        points = self.point_cloud(scanline, rover_pos, rover_rot)
        valid_chains, invalid_points = self.eval_traversability(points, rover_pos)
        paths = self.available_paths(waypoint_step, valid_chains, invalid_points, rover_pos)

        self.nodes.append(paths)

    def next_path(self):
        current_node = self.nodes[-1]
        l = [len(n) for n in self.nodes]
        print(l)
        if len(current_node) > 0:
            best_path = current_node[0]  # best path is first as list is sorted
            self.path_log.append(best_path)  # add the chosen path to the log
            del current_node[0]  # path is considered locked as it has already been travelled once

            return best_path

        else:  # if there are no available paths at this node we have to backtrack
            del self.nodes[-1]  # delete node as it is no longer viable
            backtrack = self.path_log[-1][::-1]  # reverse the last path taken
            end = self.path_log[-1][-1]
            print('Backtracking... to', end)
            return backtrack + self.next_path()
