Copyright (c) 2013 SPORT Lab(http://atrak.usc.edu/~sport/),
University of Southern California

SIMES
=====

A simulator for hybrid electrical energy storage systems.

Requirement:

1. GCC compiler or equivalent.

2. Boost library. Make sure to set enviroment variable "BOOST_ROOT" to the root directory of the boost library.

3. QT for Visualizer.

SIMES includes:

1. Simulator:

    To run Simulator, go to simulator folder, type
    
        make
        
    to compile simulator. This creates a library file in lib and an executable file in bin.

2. Visualizer:
 
    To run Visualizer, go to visualizer folder, type

        qmake
        
    first to invoke QT's qmake program to create the makefile. Then type
    
        make
        
    to compile visualizer. This creates an executable file in bin.
