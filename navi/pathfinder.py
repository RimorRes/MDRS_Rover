import math
import numpy as np


def point_coords(distance, inclination, azimuth, sensor_pos):
    x = distance * math.sin(azimuth) * math.sin(inclination)
    y = distance * math.cos(azimuth) * math.sin(inclination)
    z = distance * math.cos(inclination)
    return np.array([x, y, z]) + sensor_pos


def slope(point1, point2):
    horz = math.sqrt((point2[0] - point1[0]) ** 2 + (point2[1] - point1[1]) ** 2)
    vert = point2[2] - point1[2]
    return vert/horz


def generate_point_cloud(raw_scanline, sensor_pos, sensor_inclination):
    # TODO: might need to take into account sensor cone_angle
    point_cloud = []
    for data_point in raw_scanline:
        point_cloud.append(point_coords(
            data_point[1],
            sensor_inclination,
            data_point[0],
            sensor_pos
        ))
    return point_cloud


def find_traversable(points, max_slope, min_width):
    # TODO: take into account rover inclination
    chains = []
    c = []
    for i in range(len(points)-1):
        # evaluating 'sideways' slope between 2 points
        # evaluating if terrain is too steep or is a wall by looking at slope between from the rover head to the point
        slp = max(
            abs(slope(points[i], points[i+1])),
            abs(slope([0, 0, 0], points[i])),
        )
        if slp <= max_slope:
            c.append(points[i])
        else:
            if len(c) == 0:
                chains.append(c)
            c = []

    # culling narrow chains
    # TODO: rover might need to align itself with normal of traversable zone. Ex: tilted doors
    for c in chains:
        endpoint1, endpoint2 = c[0], c[-1]
        width = math.sqrt((endpoint2[0]-endpoint1[0])**2 + (endpoint2[1]-endpoint1[1])**2)
        if width < min_width:
            chains.remove(c)

    return chains
