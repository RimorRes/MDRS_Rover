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
rover_pos = np.array([0, 5, 0])

Planck = navi.Pathfinder(
    target=np.array([0, 10, 0]),
    sensor_pos=sensor_pos,
    sensor_inclination=sensor_inclination,
    min_width=min_width,
    max_angle=max_angle,
    step_size=0.05,
)
Planck.evaluate_terrain(
    scanline=scanline,
    rover_pos=rover_pos,
    rover_rot=np.array([0, 0, 0])
)
print(Planck.next_path())

# Plotting
print('='*30)
points = np.array(Planck.point_cloud(scanline, rover_pos))
valid_chains, invalid_points = Planck.eval_traversability(points, rover_pos)
paths = Planck.available_paths(valid_chains, invalid_points, rover_pos, np.array([0, 0, 0]))

fig = plt.figure()
ax1 = fig.add_subplot(221, projection='3d')
ax2 = fig.add_subplot(222, projection='3d')
ax3 = fig.add_subplot(223)

x, y, z = np.around(points.T, 2)
ax1.plot(x, y, z, 'o-c')
ax1.set_xlabel('X')
ax1.set_ylabel('Y')
ax1.set_zlabel('Z')

for c in valid_chains:
    x, y, z = np.around(np.array(c).T, 2)
    ax2.plot(x, y, z, '-g')
    mid_x, mid_y, mid_z = c[len(c)//2]
    ax2.plot(mid_x, mid_y, mid_z, 'ob')
x, y, z = np.around(np.array(invalid_points).T, 2)
ax2.scatter(x, y, z, c='#f23b2e')
ax2.set_xlabel('X')
ax2.set_ylabel('Y')
ax2.set_zlabel('Z')

for p in paths:
    x, y = np.array(p).T
    ax3.plot(x, y, '-ob')
x, y = np.array(invalid_points).T[:2]
ax3.scatter(x, y, c='#f23b2e')
ax3.set_xlabel('X')
ax3.set_ylabel('Y')

plt.show()
