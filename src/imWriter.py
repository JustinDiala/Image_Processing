from PIL import Image
import numpy as np

width, height = map(int, input().split())
A = np.zeros((width, height), dtype=(int,3))

for i in range(width):
    for j in range(height):
        A[i][j] = tuple(map(int, input().split()))
im = Image.fromarray(A.astype(np.uint8))
im.save("ans.jpg")

