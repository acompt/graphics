Adam Piel and Andrea Compton
apiel02       acompt01

ASSIGNMENT 1: SHAPES
COMP 175: COMPUTER GRAPHICS -- SPRING 2015
ASSIGNMENT DUE: MONDAY FEBRUARY 16, 2015


README

Files in submission:

    README
    compile
    Assignment1.cpp
    Shape.h
    Cube.h
    Cube.cpp
    Cylinder.h
    Cylinder.cpp
    Cone.h
    Cone.cpp
    Sphere.h
    Sphere.cpp
    Coil.h
    Coil.cpp
    CubeList.h
    CubeList.cpp
    FaceList.h
    FaceList.cpp
    Algebra.h


TO COMPILE:

To compile, simply run "sh compile." The following cpp files need to be compiled in order to properly compile our program: Assignment1.cpp, FaceList.cpp, Cube.cpp, CubeList.cpp, Cylinder.cpp, Cone.cpp, Sphere.cpp, and Coil.cpp.


CODE OVERVIEW:

We chose to do a coil as our "special" shape. Shaped like a spring with three loops, the coil fits within the unit cube.

We employed several helper classes for storing information about our shapes. The cube employs a specialized data structure we called "CubeList," and the other shapes use a more generalized data structure called "FaceList."

In all cases, vertices and faces are calculated and stored in the data structure. Then, when it is required that normals be drawn, the normals can be read from the data structure and drawn without any recalculation.

There are no known bugs in our code; to the best of our knowledge everything works as specified in the assignment handout. 