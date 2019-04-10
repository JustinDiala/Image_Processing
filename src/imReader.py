import os, sys
from PIL import Image
import numpy as np

im = Image.open(input())
width, height = im.size

pixel = list(im.getdata())

print(width, height)
print(*map(lambda x: "%d %d %d" % x, pixel), sep="\n")


im = Image.open(input())
width, height = im.size

pixel = list(im.getdata())

print(width, height, sep="\n")
print(*map(lambda x: "%d %d %d" % x, pixel), sep="\n")
