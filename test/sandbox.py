import numpy as np
from matplotlib import pyplot as plt
from shapely import geometry
import json
import navi


SENSOR_MAX = 4

def load_world(filepath):
    with open(filepath, 'r') as file:
        vertices = json.load(file)
    polygons = []
    for l in vertices:
        polygons.append(geometry.Polygon(l))
    return polygons


def ray_cast(origin, vec, world_obstacles, step=0.01):
    dist = 0
    while dist <= SENSOR_MAX:
        ray_end = geometry.Point(origin + vec*dist)
        for poly in world_obstacles:
            if ray_end.intersects(poly):
                return dist
        dist += step
    return np.inf


def scan(pos, world, num_steps=180):
    scanline = []
    for i in range(num_steps+1):
        a = i/num_steps * np.pi
        dist = ray_cast(
            pos,
            np.array([np.cos(a), np.sin(a)]),
            world
        )

        scanline.append([
            dist,
            a
        ])
    return scanline

w = load_world('map.json')
s = scan(
    np.array([0, 0]),
    w
)
points = navi.process_scanline(s)
obstacles = navi.obstacle_polygons(points, 0.20)
grid = navi.gen_grid(obstacles, 0.1)

# Plotting
fig, axs = plt.subplots(2, 2)

for o in w:
    x,y = o.exterior.xy
    axs[0, 0].plot(x, y)
axs[0, 0].axis('equal')

for chain in points:
    x, y = np.array(chain).T
    axs[0, 1].plot(x, y)
axs[0, 1].set(xlim=(-4, 4), ylim=(0, 4))

for o in obstacles:
    b = o.buffer(-0.20)
    x,y = o.exterior.xy
    bx, by = b.exterior.xy
    axs[1, 0].plot(x, y)
    axs[1, 0].plot(bx, by)

axs[1, 1].imshow(grid)

plt.show()