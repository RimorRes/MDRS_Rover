import numpy as np
from shapely import geometry


SENSOR_MAX = 4


def normalize(v):
    norm = np.linalg.norm(v)
    if norm == 0:
        return v
    return v / norm


def sphere2polar(distance, angle):
    x = distance * np.cos(angle)
    y = distance * np.sin(angle)
    return np.array([x, y])


def process_scanline(scanline):
    # TODO: Smooth scanline
    point_chains = []
    chain = []
    for distance, angle in scanline:
        if distance <= SENSOR_MAX:
            chain.append(sphere2polar(
                distance,
                angle
            ))
        else:
            if len(chain) > 1:
                point_chains.append(chain)
            chain = []

    if len(chain) > 1:
        point_chains.append(chain)

    return point_chains


def obstacle_polygons(obstacle_chains, clearance):
    polygons = []
    for chain in obstacle_chains:
        projections = []
        for p in chain:
            projections.insert(0, normalize(p) * SENSOR_MAX)
        vertices = chain + projections
        polygons.append(geometry.Polygon(vertices).buffer(clearance))

    return polygons


def gen_grid(obstacles, step=0.1):
    # Build empty grid
    size = int(SENSOR_MAX/step)
    grid = np.zeros((size, 2*size))

    # Draw circle
    for y in range(grid.shape[0]):
        for x in range(grid.shape[1]):
            for poly in obstacles:
                if geometry.Point(x*step-SENSOR_MAX, y*step).intersects(poly):
                    grid[y, x] = 1
            if np.sqrt((x - size+0.5)**2 + y**2) > size:
                grid[y, x] = 2
    return grid


class BigNode:

    def __init__(self, pos, parent, scan):
        self.pos = pos
        self.parent = parent
        self.scan_points = scan
