import math
import numpy as np
import json
from matplotlib import pyplot as plt
from context import navi


with open("scanline.json", 'r') as f:
    data = json.load(f)
    sensor_pos, sensor_inclination, scanline = data
max_angle = math.radians(15)
min_width = 0.5

Patterson = navi.Pathfinder(
    target=np.array([0, 10, 0]),
    sensor_pos=sensor_pos,
    sensor_inclination=sensor_inclination,
    min_width=min_width,
    max_angle=max_angle
)
Patterson.evaluate_terrain(
    scanline=scanline,
    rover_pos=np.array([0, 0, 0])
)
points = np.array(Patterson.point_cloud(scanline))
chains = Patterson.valid_chains(points)

# Plotting
fig = plt.figure()
ax1 = fig.add_subplot(121, projection='3d')
ax2 = fig.add_subplot(122, projection='3d')

x, y, z = np.around(points.T, 2)
ax1.plot(x, y, z, 'o-c')
ax1.set_xlabel('X')
ax1.set_ylabel('Y')
ax1.set_zlabel('Z')

for c in chains:
    x, y, z = np.around(np.array(c).T, 2)
    ax2.plot(x, y, z, '-g')
    mid_x, mid_y, mid_z = c[len(c)//2]
    ax2.plot(mid_x, mid_y, mid_z, 'or')
ax2.set_xlabel('X')
ax2.set_ylabel('Y')
ax2.set_zlabel('Z')

plt.show()
