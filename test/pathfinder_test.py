import math
import numpy as np
import random
from matplotlib import pyplot as plt
from context import navi

sensor_pos = np.array([0, 0, 0.5])
sensor_inclination = math.radians(90+45)
obstacle = 0.2

a = np.linspace(-math.pi/2, math.pi/2, num=10)
high = sensor_pos[2]/math.cos(sensor_inclination-(math.pi/2))
low = (sensor_pos[2]-obstacle)/math.cos(sensor_inclination-(math.pi/2))
d = np.array([low if random.random() < 0.5 else high for _ in range(10)])
scanline = np.array([a, d]).T

cloud = np.array(navi.generate_point_cloud(scanline, sensor_pos, sensor_inclination))
x, y, z = cloud.T

fig = plt.figure()
ax1 = fig.add_subplot(121, projection='3d')
ax2 = fig.add_subplot(122)

ax1.plot(x, y, z, 'o-c')
ax1.set_xlabel('X')
ax1.set_ylabel('Y')
ax1.set_zlabel('Z')
ax2.plot(a, d)
plt.show()
