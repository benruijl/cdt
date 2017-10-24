cdt
===

cdt is a 1+1 dimensional Locally Causal Dynamic Triangulation Monte Carlo Simulator. A part of the standard CDT constraints are relaxed to allow for bubbles, while still adhering to the causality requirements.


Installation
==

In order to compile `cdt`, one needs a modern C++ compiler and the `boost` package. Then run:

    make CONF=Release
    
    
Running
==

You should run the executable:

    ./dist/Release/GNU-Linux-x86/cdt

You can tune the parameters of the simulation in `config.ini`.
