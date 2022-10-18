# Python code for Multiple Color Detection

import serial
import time
import numpy as np
import cv2
import math


mingreen=10
minred =10
counter = 0
limitCounter = 30
ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
ser.reset_input_buffer()
# Capturing video through webcam
webcam = cv2.VideoCapture(0)
redarea = 0
greanarea = 0
KNOWN_DISTANCE = 94  # centimeter
KNOWN_WIDTH = 5  # centimeter
# Start a while loop
def FocalLength(measured_distance, real_width, width_in_rf_image):
    focal_length = (width_in_rf_image* measured_distance)/ real_width
    return focal_length

def Distance_finder(Focal_Length, real_face_width, face_width_in_frame):
    distance = (real_face_width * Focal_Length)/face_width_in_frame
    return distance
while (1):
    redcct=0
    greencct=0
    redy=0
    greeny=0
    width0  = webcam.get(cv2.CAP_PROP_FRAME_WIDTH)

   # print (width0)
    hight0  = webcam.get(cv2.CAP_PROP_FRAME_HEIGHT)

   # print (hight0)
    # Reading the video from the
    # webcam in image frames
    _, imageFrame = webcam.read()
#    imageFrame = cv2.resize(imageFrame, (800, 300))
    imageFrame = imageFrame[210:,:]
    # Convert the imageFrame in
    # BGR(RGB color space) to
    # HSV(hue-saturation-value)
    # color space
    hsvFrame = cv2.cvtColor(imageFrame, cv2.COLOR_BGR2HSV)

    # Set range for red color and
    # define mask
    red_lower = np.array([170, 100, 50], np.uint8)
    red_upper = np.array([180, 255, 255], np.uint8)
    red_mask = cv2.inRange(hsvFrame, red_lower, red_upper)

    # Set range for green color and
    # define mask
    green_lower = np.array([50,100, 50], np.uint8)
    green_upper = np.array([80, 255, 255], np.uint8)
    green_mask = cv2.inRange(hsvFrame, green_lower, green_upper)



    # Morphological Transform, Dilation
    # for each color and bitwise_and operator
    # between imageFrame and mask determines
    # to detect only that particular color
    kernal = np.ones((5, 5), "uint8")

    # For red color
    red_mask = cv2.dilate(red_mask, kernal)
    res_red = cv2.bitwise_and(imageFrame, imageFrame,
                              mask=red_mask)

    # For green color
    green_mask = cv2.dilate(green_mask, kernal)
    res_green = cv2.bitwise_and(imageFrame, imageFrame,
                                mask=green_mask)



    # Creating contour to track red color
    contours, hierarchy = cv2.findContours(red_mask,
                                           cv2.RETR_TREE,
                                           cv2.CHAIN_APPROX_SIMPLE)

    for pic, contour in enumerate(contours):
        redcct=redcct+1
        area = cv2.contourArea(contour)
        redarea = area
        pt1 = (320, 0)
        pt2 = (320, 480)

        color = (255, 255, 0)
        cv2.line(imageFrame, pt1, pt2,color)
        if (area > 300):
            x, y, w, h = cv2.boundingRect(contour)
            redarea = w * h
           # print (w)
            dd= int(Distance_finder(60.16,5,w/10))
            pt1 = (x, y)
            pt2 = (320, 480)
            if x>320 :
                pt1 = (x+w, y)
            pt3 = (320,y)
           # print (pt1)
            slope = (480-y) / (320 -x)
            color = (0, 0, 255)
            cv2.line(imageFrame, pt1, pt2,color)
            cv2.line(imageFrame, pt3, pt1,color)
            imageFrame = cv2.rectangle(imageFrame, (x, y),
                                       (x + w, y + h),
                                       (0, 0, 255), 2)


            z=0
            if x>320 :
                z=x-320+w
            else:
                z=320-x
            theta = math.degrees(math.atan((z/10)/dd))
            cv2.putText(imageFrame, "L="+str(z/10) , (x+int((z/2)), y+30),
                        cv2.FONT_HERSHEY_SIMPLEX, 1.0,
                        (0, 255, 255))
            cv2.putText(imageFrame, "R d="+str(dd) +" @="+str(int(theta)), (x, y),
                        cv2.FONT_HERSHEY_SIMPLEX, 1.0,
                        (0, 255, 255))

    # Creating contour to track green color
    contours, hierarchy = cv2.findContours(green_mask,
                                           cv2.RETR_TREE,
                                           cv2.CHAIN_APPROX_SIMPLE)

    for pic, contour in enumerate(contours):
        greencct=greencct+1
        area = cv2.contourArea(contour)
        greanarea = area
        if (area > 300):
            x, y, w, h = cv2.boundingRect(contour)
            greanarea = h * w
            pt1 = (x, y)
            pt2 = (320, 480)
            if x>320 :
                pt1 = (x+w, y)
            pt3 = (320,y)
            slope = (480-y) / (320 -x)
            color = (0, 255, 0)
            cv2.line(imageFrame, pt1, pt2,color)
            cv2.line(imageFrame, pt3, pt1,color)
            dd=int( Distance_finder(60.16,5,w/10))
            imageFrame = cv2.rectangle(imageFrame, (x, y),
                                       (x + w, y + h),
                                       (0, 255, 0), 2)


            z=0
            if x>320 :
                z=x-320 +w
            else:
                z=320-x
            theta = math.degrees(math.atan((z/10)/dd))

            cv2.putText(imageFrame, "L="+str(z/10) , (x+int(z/2), y+30),
                        cv2.FONT_HERSHEY_SIMPLEX, 1.0,
                        (0, 255, 255))
            cv2.putText(imageFrame, "G d="+str(dd) +" @="+str(int(slope)), (x, y),
                        cv2.FONT_HERSHEY_SIMPLEX,
                        1.0, (0, 255, 0))

    counter = counter +1


    if greencct < mingreen :
        mingreen =greencct
    if redcct < minred :
        minred =redcct



    state = "0"
    if mingreen >0 and minred >0 and greanarea > redarea :
        state ="8"
        if counter > limitCounter :
          #  ser.write(b"8\n")
           # line = ser.readline().decode('utf-8').rstrip()
            #print(line)
            counter =0
            mingreen=10
            minred =10

    elif mingreen >0 and minred >0 and greanarea < redarea :
        state ="7"
        if counter > limitCounter :
          #  ser.write(b"7\n")
          #  line = ser.readline().decode('utf-8').rstrip()
           # print(line)
            counter =0
            mingreen=10
            minred =10

    elif mingreen > 1 and minred == 0 :
        state ="6"
        if counter > limitCounter :
         #   ser.write(b"6\n")
         #   line = ser.readline().decode('utf-8').rstrip()
          #  print(line)
            counter =0
            mingreen=10
            minred =10

    elif  minred >1 and mingreen ==0 :
        state ="3"
        if counter > limitCounter :
            #ser.write(b"3\n")
           # line = ser.readline().decode('utf-8').rstrip()
           # print(line)
            counter =0
            mingreen=10
            minred =10

    elif mingreen == 1 and greanarea > 2000 and minred == 0 :
        state ="4"
        if counter > limitCounter :
            #ser.write(b"4\n")
            #line = ser.readline().decode('utf-8').rstrip()
            #print(line)
            counter =0
            mingreen=10
            minred =10

    elif  minred == 1 and redarea > 2000 and mingreen ==0:
        state ="1"
        if counter > limitCounter :
            ser.write(b"1\n")
            line = ser.readline().decode('utf-8').rstrip()
            print(line)
            counter =0
            mingreen=10
            minred =10
            print("1")

    elif mingreen == 1 and greanarea < 2000 and minred==0 :
        state ="5"
        if counter > limitCounter :
            #ser.write(b"5\n")
            #line = ser.readline().decode('utf-8').rstrip()
            #print(line)
            counter =0
            mingreen=10
            minred =10

    elif  minred == 1 and redarea < 2000  and mingreen ==0:
        state ="2"
        if counter > limitCounter :
            #ser.write(b"2\n")
            #line = ser.readline().decode('utf-8').rstrip()
            #print(line)
            counter =0
            mingreen=10
            minred =10



    else:
     #   if counter > limitCounter :
            #ser.write(b"9\n")
        state="9"
        counter =0
        mingreen=10
        minred =10

    greencct = 0
    redctt=0




    # Program Termination
    cv2.imshow("Multiple Color Detection in Real-TIme", imageFrame)
    if cv2.waitKey(10) & 0xFF == ord('q'):
        cap.release()
        cv2.destroyAllWindows()
        break
