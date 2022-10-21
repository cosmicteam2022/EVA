
# Introduction #

The vehicle EVA is designed using two microcontrollers: Raspberry Pi, and Arduino. The Arduino is programmed on a Linux environment, which is installed on Raspberry pi, Arduino is connecting to the Raspberry Pi using a serial port, and the platform that is used for programming and uploading the code on Arduino is Arduino IDE ARM version.

Depending on the previous, the C++ language is used for Arduino, and the Python language is used for Raspberry pi, the C++ language is used to program the Arduino microcontroller, to control the movement of the motor, steering wheel, and sensors (Ultrasonic, gyroscope), while the python is used for image processing depending on the suitable library so that the Open CV v4.5.1 library has been downloaded suitable to python. it is used for processing the image coming from the camera, clustering the red and green pillars, and sending commands for each case according to the color.

In a deeper look, the code is discussed in detail.

## C++ coding: ##

The first section consists of the libraries used to control the servo motor movement, the gyro sensor, and the time library.

The second section consists of global variables for the main and sub-functions, that carried names with programmatic significance, such as the normalSpeed variable to determine the vehicle's speed, and the turnSpeed.... etc.

The third section consists of functions, and the most important of these functions are:

turnLeft (), turnRight (), the direction to the left and the right are determined by Ultrasonic Sensors, and this is performed according to the following: When the vehicle turns counter-clockwise, the left Ultrasonic sensor value is large, and when you turn clockwise, the right one is large. also, the getDistance () function is used for reading whole ultrasonic sensors, in some cases getForward (), getBack (), getRight (), and getLeft () functions are used to read each ultrasonic separately.

The required configurations are adjusted within the setup () function and the main code was also written inside the loop () function. It is worth noting that the gyroscope sensor was used to control the vehicle's rotation at the required angle, and a function is created for the delay so that it keeps updating the values ​​​​created by this sensor without any defect, and it is also added A function for tuning the vehicle's path so that it is as much as possible in the middle of the path, in addition to other functions such as controlling the rotation of the servo motor that is used in the steering and so on.

The procedures which are mentioned above are used for the game without pillars, but in the case of the presence of the pillars, some fundamental changes have been made to the code, which are:

The rotation method has been processed using the straightBack () function, in addition to adding a function to read the serial that takes its orders from the Raspberry Pi controller, in addition to adding the cases that the vehicle must do in the case of seeing the green and red pillars according to the requirements of the game. Other functions have also been added to achieve the rotation of the vehicle to any degree, as well as the angle of the steering.

## Python coding: ##

The pre-existing Python language on Raspberry Pi's operating system was used to operate the camera and process the images taken from it.

**First** , we converted the color scheme of the image from RGB to HSV.

**Second** , we created a color mask with which we determined the color range of green and red in the HSV system.

 **Third** , we used the functions supported by the OpenCV library to extract the regions that contain these masks in the image.

**Fourth**  , we used functions to extract the area depending on ​​these masks, taking into account their rectangular shape.

**Fifth**  , we calculated the area for these masks and ignored areas less than 300 pixels.

**Sixth** , we create global variables to count the number of masks of both colors.

**Seventh** , the least possible number of these masks of both colors was used to repeat 10 times to reduce the noise that might occur during the live video.

**Eighth** , we connected the Raspberry Pi controller to the Arduino through the serial port including the serial library.

**Ninth** , we sent the commands through the serial port of the Arduino so that the Arduino takes the appropriate action.



>**Important notes:**
>The errors that occur are addressed through the practical implementation of electronic parts and electromechanical parts of trial and error. A special coupling has been created for the gyroscope in order to update the gyroscope values and also to give the gyroscope in a way that ensures the immediate update of all the variables. Also, the problems that occur during the reading of the gyroscope are calibrated by experiment. We found that the gyroscope gives the value of the angle 87 when it rotates 90 degrees and this is in counter-clockwise, but on clockwise the gyroscope gives a value of 85 degrees.
>One of the problems that we also encountered is that the gyroscope does not zero the values after 360 degrees, so we deal with this matter programmatically by creating variables and functions for that. We also encountered a problem in calibrating the steering wheel, as it needs to be calibrated every period of time, as it does not adjust itself to a 90-degree angle.

