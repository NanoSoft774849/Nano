import cv2 as cv
#import matplotlib as plt
import numpy as np
import math
import os
import time

import cv2.data;


#Name : Adulbary
#email : abdulbaryhowbani@gmail.com
#university of science and Technology of China (USTC)


# magic constant by experiments.
magic_constant = 100.0/294.0
#convert Image to Gray.
def toGray(imgx):
    return cv.cvtColor(imgx,cv.COLOR_BGR2GRAY)

#match Template.
def ns_find_template(img_path , template_path):

    
    img = cv.imread(img_path)
    if(  img.shape == None):
        print("image is empty")
        return (0,(0,0))
    template = cv.imread(template_path)
    if(  template.shape == None):
        print("template is not exist")
        return (0,(0,0))

    # matching method using Cross Correlation
    match_method = cv.TM_CCORR_NORMED
    #apply template matching to find the location of template in the Image.

    xmatch =cv.matchTemplate(img,template,cv.TM_CCORR_NORMED)
    minVal1, _maxVal1, minLoc, maxLoc = cv.minMaxLoc(xmatch ,None)
    print("maxlocation:",maxLoc)
    return (_maxVal1 , maxLoc)

#max
def ns_max ( x, y) :
    return x if x>=y else y

#min
def ns_min( x, y) :
    return x if x<=y else y


#calc , xdiff , ydiff , ecludian distance.
def calc_distance( p1, p2 ):

    x0 = p1[0] 
    x1 = p2[0]
    y0 = p1[1]
    y1 = p2[1]
    #horizontal difference 
    xdiff = abs( x1 - x0) * magic_constant # distance in mm
    #vertical difference
    ydiff =abs ( y1 - y0) * magic_constant # distance in mm
    #ecludian difference 
    diff = math.sqrt( (xdiff * xdiff) +( ydiff * ydiff))

    return (xdiff , ydiff, diff)


def test():

    ref_path = "images/ref0.jpg";
    test_img= "images/apptest.jpg";
    template_path ="images/template.pgm";

    m, p1 = ns_find_template(test_img , template_path)
    m2, p2 = ns_find_template(ref_path, template_path)

    xdif,ydif , edif = calc_distance(p1, p2)
    print("horizontal shift(mm):", xdif)
    print("vertical shift (mm):", ydif)
    print("e shift(mm):",edif)

    #ref = cv.imread(ref_path)

    #cv.imshow("ref image",ref)
    #cv.waitKey(0)
def ProcessImages(ref_path , template_path  , test_img):

    m, p1 = ns_find_template(test_img , template_path)
    m2, p2 = ns_find_template(ref_path, template_path)

    xdif,ydif , edif = calc_distance(p1, p2)
    print("horizontal shift(mm):", xdif)
    print("vertical shift (mm):", ydif)
    print("e shift(mm):",edif)

     

import sys
import os 

def isExists(str ):
    return os.path.exists(str)

if __name__ =="__main__":

    argc = len(sys.argv)
    #--def arguement used if you want to use default path for images.
    if(len(sys.argv)<4):
        print("Usage main.py ref_image template_img test_img [--def] ")
        exit()


    ref_img = sys.argv[1]
    template_img = sys.argv[2]
    test_img =  sys.argv[3]
    _def = False;
    def_path ="images/"
    if( argc == 5):
       _def = sys.argv[4] =='--def';

    if ( _def ) :
        test_img = def_path+test_img
        ref_img = def_path +ref_img
        template_img = def_path+template_img

    if not os.path.exists(ref_img):
        print("ref image is not exist!")
        exit()


    
    if not isExists(template_img):
        print( " template image no exists ");
        exit()
   
    if( not isExists(test_img)):
        print("test image is not exist !")
        exit()
    
     
    for arg in sys.argv:
        print(arg)

    # apply algorithm for images .
    ProcessImages(ref_img  , template_img , test_img)
    
 


    print( "done ...")

