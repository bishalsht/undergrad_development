#bitplane splicing
import numpy as np 
import cv2
import StringIO
from PIL import Image
import urllib2 as lb
import urllib
import requests

def bitplane(image,plane):
    for i in range(image.shape[0]):
        for j in range(image.shape[1]):
            if image[i,j]&plane:
                image[i,j]=255
            else:
                image[i,j]=0
        #     print image[i,j],
        # print "\n\n\n"
    cv2.imwrite('/tmp/'+str(plane)+'lol.jpg',image)    
    

# fp = urllib.urlopen("http://localhost:50070/webhdfs/v1/user/hduser/input/input_image3.jpg?op=OPEN") 
fp = urllib.urlopen("/home/bishal/Desktop/abc.jpg")
img = StringIO.StringIO(fp.read())  
temp=Image.open(img)
# temp=Image.open("/home/bishal/Desktop/abc.jpg")
img=np.array(temp)
grayimage = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
cv2.imwrite('/home/bishal/Desktop/turu.jpg', grayimage)
plane = 1
# cv2.imwrite('/tmp/read.jpg',grayimage)

# for i in range(img.shape[0]):
#     for j in range(img.shape[1]):
#         print img[i,j],
#     print "\n\n\n\n\n\n\n"


for i in range(0,8):
    print "bitplane ",i+1
    temp = np.copy(grayimage)
    bitplane(temp,plane<<i)
raw_input("Press Enter to continue...")

