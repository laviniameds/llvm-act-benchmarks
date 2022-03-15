from __future__ import division
from pyglet import image
import sys

#IMGDIR = 'saved_outputs'

#def load():
#   return 'file:out.pgm'

def score(orig, relaxed):
    orig_image = image.load(orig)
    relaxed_image = image.load(relaxed)
    error = 0
    total = 0

    try:
        orig_data = orig_image.get_image_data()
        relaxed_data = relaxed_image.get_image_data()
    except ValueError:
        print('error')
        return 1.0

    for ppixel, apixel in zip(orig_data.get_data(), relaxed_data.get_data()):
        error += abs(ppixel - apixel)
        total += 1
    return error / 255 / total
    
print(score(sys.argv[1], sys.argv[2]))
