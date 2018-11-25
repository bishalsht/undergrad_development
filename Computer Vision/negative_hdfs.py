import cv2
import numpy as np 
import StringIO
from PIL import Image
import urllib2 as lb
import urllib

# fp = urllib.urlopen("http://localhost:50070/webhdfs/v1/user/hduser/input/input_image3.jpg?op=OPEN") 
fp = urllib.urlopen("/home/bishal/Desktop/abc.jpg")
img = StringIO.StringIO(fp.read())  
temp=Image.open(img)
img=np.array(temp)
grayimage = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
cv2.imshow("image", grayimage)

cv2.imshow("negative",255-grayimage)
cv2.waitKey(0)
raw_input("Press Enter to continue...")
