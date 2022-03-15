from __future__ import division
from math import sqrt
# import PIL
# from PIL import ImageFile, Image
# ImageFile.LOAD_TRUNCATED_IMAGES = True
from pyglet import image
import itertools
import sys

IMGDIR = 'saved_outputs'

# def load():
#     return 'file:baboon.rgb.jpg'

def score(orig, relaxed):
    print ('Orig: %s' % orig)
    orig_image = image.load(orig)
    print ('Relaxed: %s' % relaxed)
    relaxed_image = image.load(relaxed)
    error = 0
    total = 0

    try:
        orig_data = orig_image.get_image_data()
        relaxed_data = relaxed_image.get_image_data()
    except ValueError:
        return 1.0

    for ppixel, apixel in zip(orig_data.get_data(), relaxed_data.get_data()):
        # root-mean-square error per pixel
        # print('ppixel: {0} - apixel: {1}'.format(ppixel, apixel))
        # pxerr = sqrt((ppixel - apixel)**2) / 255
        pxerr = sqrt(((ppixel - apixel)**2 +
                      (ppixel - apixel)**2 +
                      (ppixel - apixel)**2) / 3) / 255
        error += 1.0 - pxerr
        total += 1.0
    if pxerr == 0.0:
        error = 0.0
    #print('error: {0} - total: {1} - pxerr: {2}'.format(error, total, pxerr))
    return error / total

print (score(sys.argv[1], sys.argv[2]))