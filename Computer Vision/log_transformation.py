import numpy as np 
import cv2
import StringIO
from PIL import Image
import urllib2 as lb
import urllib
import requests
import math

def findLog(image,c):
    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            x = math.log(image[i,j]+1)
            image[i,j] = c*x
    cv2.imshow("image",image)
    # cv2.imwrite("/home/bishal/Desktop/logimage", image)

fp = urllib.urlopen("/home/bishal/Desktop/image2.tif")
img = StringIO.StringIO(fp.read())  
temp=Image.open(img)
img=np.array(temp)
c = 30

cv2.imshow("original", img)
findLog(img, c)

cv2.waitKey(0)
raw_input("Press Enter to continue...")

