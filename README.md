# RAW12toPPM
This repository extracts 4 channels (red, green1, green2 and blue) from a RAW12 image and saves them in PPM format. 

### To Do List
- [x] Separate 4 channels (red, green1, green2 and blue)
- [x] Output Intensity values of 5x5 pixels
- [x] Save the 4 channels separately without the use of any external library
- [x] Debayer the CFA
- [x] Save the debayered image without the use of any external library
- [ ] Save the debayered image as BMP file, without the use of any external library
- [ ] Use CMake to build the CPP program
- [ ] Abide by C/CPP coding guidlines and project structuring (https://gist.github.com/lefticus/10191322)
- [ ] Load a part of program as dynamic library (.so file)
- [ ] Use non linear curve for 12 bits to 8bits conversion

### Build Instructions
```
mkdir result
mkdir build
cd build
cmake ..
make
./run
```
### Debayering Algorithm Comparison

Bilinear                   |  Nearest Neighbour
:-------------------------:|:-------------------------:
<img src="https://drive.google.com/uc?export=view&id=1Gi5BYspwMC169NZ9BKIA-vacbk-EpUsu" width="400">  |  <img src="https://drive.google.com/uc?export=view&id=1g3L0NRcnk0svcdVnBnP4Y2BHN8DpsvN8" width="400">

**Notice the subtle aliasing effect for Nearest Neighbour near the horizontal lines of steel rod**
