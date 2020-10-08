# About Runaway Fluid
Repository for the Runaway Fluid Kepler actor for integration into [IMAS](https://core.ac.uk/download/pdf/207980869.pdf) and [ETS](https://wpcd-workflows.github.io/). 

# How to clone repository
The latest release of the code should be checked out using

`git clone --depth 1 --branch <tag_name> https://github.com/osrep/Runafluid.git`.

# How to build
This code can only be run and compiled on ITM or IMAS environments. To build simply type:

`./build.sh`

If you want to build and test type:

`./test.sh`

# High level summary of functions
Indicating the runaway electrons and calculating their density and current:

1. Indicate, whether the runaway generation is possible
2. Calculate the runaway density and current
3. Indicate, whether the runaway generation is over critical_fraction 
4. Indicate, whether the runaway current is over the total electron current (modell error indicator)

# Detailed documentation
Detailed documentation can be found [here](https://github.com/osrep/Runafluid/tree/master/doc)

# References
To cite Runaway Fluid as a tool please use the a following paper:

 G.I. Pokol, et. al, Runaway electron modelling in the self-consistent core European Transport Simulator, ETS, Nuclear Fusion 59, 076024 (2019)
