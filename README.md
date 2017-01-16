# Computer-Vision
A number of computer vision algorithms done in my Computer Vision course at UCF

# How to run Sobel
In terminal: 
gcc sobel.c -o sobel -lm
 ./sobel face05.pgm output1.pgm output2.pgm output3.pgm <threshold1(0-100)> <threshold2(0-100)>

# How to run Canny
gcc canny.c -o canny -lm
./canny garb34.pgm output1.pgm output2.pgm output3.pgm <threshold1(0-1)> <threshold2(0-1)>