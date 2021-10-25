import math
import numpy as np
import json
from matplotlib import pyplot as plt
from context import navi


with open("scanline.json", 'r') as f:
    data = json.load(f)
    sensor_pos, sensor_inclination, scanline = data

cloud = np.array(navi.generate_point_cloud(scanline, sensor_pos, sensor_inclination))

chains = navi.find_traversable(cloud, math.radians(15), 0.5)

fig = plt.figure()
ax1 = fig.add_subplot(121, projection='3d')
ax2 = fig.add_subplot(122, projection='3d')

x, y, z = np.around(cloud.T, 2)
ax1.plot(x, y, z, 'o-c')
ax1.set_xlabel('X')
ax1.set_ylabel('Y')
ax1.set_zlabel('Z')

for c in chains:
    x, y, z = np.around(np.array(c).T, 2)
    ax2.plot(x, y, z, '-g')
ax2.set_xlabel('X')
ax2.set_ylabel('Y')
ax2.set_zlabel('Z')


# TODO: Mask points behind walls and fix max obstacle height

plt.show()
