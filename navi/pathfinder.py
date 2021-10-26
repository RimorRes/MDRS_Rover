import math
import numpy as np


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
    return rise/run


class Pathfinder:

    def __init__(self, target, sensor_pos, sensor_inclination, min_width, max_angle):
        self.target = target

        # Path picking constraints
        self.min_width = min_width
        self.max_angle = max_angle
        self.sensor_pos = sensor_pos  # sensor position relative to the rover
        self.sensor_inclination = sensor_inclination   # sensor tilt (on +X) relative to +Z axis

        self.forks = []  # list of past encountered forks each fork containing untested paths

    def point_cloud(self, scanline):
        # TODO: might need to take into account sensor cone_angle
        point_cloud = []  # list of all scanned points
        for data_point in scanline:
            coords = sphere2cartesian(
                data_point[1],
                self.sensor_inclination,
                data_point[0],
            )
            point_cloud.append(coords + self.sensor_pos)
        return point_cloud

    def valid_chains(self, points):
        # TODO: take into account rover inclination
        potential_chains = []
        c = []
        for i in range(1, len(points) - 1):
            # evaluating 'sideways' slope between 2 points
            # evaluating if terrain is too steep or is a wall by looking at slope between the rover head to the point
            slopes = [
                abs(slope(points[i - 1], points[i])),
                abs(slope(points[i], points[i + 1])),
                abs(slope([0, 0, 0], points[i])),
            ]
            if max(slopes) <= math.atan(self.max_angle):
                c.append(points[i])
            else:  # traversable chain ended, time to save it
                if len(c) > 1:  # don't want empty lists or single point lists
                    potential_chains.append(c)
                c = []  # empty chain
        # checking last chain
        if len(c) > 1:  # don't want empty lists or single point lists
            potential_chains.append(c)

        # culling narrow chains
        # TODO: rover might need to align itself with normal of traversable zone. Ex: tilted doors
        chains = []
        for c in potential_chains:
            endpoint1, endpoint2 = c[0], c[-1]
            width = math.sqrt((endpoint2[0] - endpoint1[0]) ** 2 + (endpoint2[1] - endpoint1[1]) ** 2)
            if width > self.min_width:
                chains.append(c)

        # Debug
        print('found', len(potential_chains), 'potential chains')
        print('found', len(chains), 'valid chains')

        return chains

    def available_paths(self, chains, rover_pos):
        target_heading = normalize(self.target - rover_pos)
        paths = []
        for c in chains:
            mid = c[len(c)//2]
            path_heading = normalize(mid)
            alignment = np.dot(target_heading, path_heading)
            paths.append([mid, alignment])

        paths.sort(key=lambda x: x[1], reverse=True)

        # Debug
        print('All paths at fork:')
        for p in paths:
            print('- alignment:', p[1], '\twaypoint:', p[0])

        return paths

    def evaluate_terrain(self, scanline, rover_pos):
        points = self.point_cloud(scanline)
        chains = self.valid_chains(points)
        fork = self.available_paths(chains, rover_pos)

        self.forks.append(fork)

# TODO: Mask points behind walls and fix max obstacle height
