import navi
import numpy as np
from matplotlib import pyplot as plt
from PIL import Image, ImageOps


img = ImageOps.invert(Image.open('map.png').convert('L'))
graph = np.array(img)
s = 0
for y in range(graph.shape[0]):
    for x in range(graph.shape[1]):
        if graph[y, x] == 0:
            s += 1
print(s)

print(graph.shape)
# graph = np.array([
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 1, 0, 1, 1, 0, 0, 0, 0],
#     [0, 0, 1, 0, 0, 1, 0, 0, 0, 0],
#     [0, 0, 1, 0, 0, 1, 0, 0, 0, 0],
#     [0, 0, 1, 0, 0, 1, 0, 0, 0, 0],
#     [1, 1, 1, 1, 1, 1, 1, 0, 0, 0],
#     [0, 0, 0, 0, 0, 1, 1, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
#     [0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
# ])
plt.imshow(graph)
path, analyzed = navi.a_star(
    graph,
    np.array([0, 0]),
    np.array([77, 8])
)
print("Analyzed {} total nodes".format(analyzed))
for p in path:
    x, y = p
    graph[y, x] = 128
plt.imshow(graph)