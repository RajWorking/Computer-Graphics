# Hello-World

## Run the program

```
mkdir build
cd build
cmake ..
make
./HelloWorld
```

Choose 1,2,3 on the command prompt to select which solid you want to display.


## Controls

### Object
* W : Move the object in negative x axis (global).
* E : Move the object in positive x axis (global).
* S : Move the object in negative y axis (global).
* D : Move the object in positive y axis (global).
* X : Move the object in negative z axis (global).
* C : Move the object in positive z axis (global).

### Camera
* T : Move the camera in negative x axis (relative).
* Y : Move the camera in positive x axis (relative).
* G : Move the camera in negative y axis (relative).
* H : Move the camera in positive y axis (relative).
* B : Move the camera in negative z axis (relative).
* N : Move the camera in positive z axis (relative).

### General
* F : Turn the camera to look at the object
* R : Rotate the camera around the object.
* Z : Rotate the object around x axis (global) anticlockwise.
* A : Rotate the object around x axis (global) clockwise.
* P : Print some information for debugging