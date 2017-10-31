# MarbleSphereSimulation
A real-time simulation of a marble rolling inside a bowl (represented as a wireframe sphere) with OpenGL

# How to Build and Run
First clone the repository. Remember to add the recursive option as there is a submodule that must also be cloned.

`git clone --recursive https://github.com/lscholte/MarbleSphereSimulation.git`

If you forget to add the recursive option, then execute the following from within your local copy of the repository:

`git submodule update --init --recursive`

Next, create an empty build directory wherever you like. It is suggested to create this directory outside of your local copy of the repository. Once created, navigate to the directory and execute

`cmake <path to root of repository>`

`cmake --build .`

If the build was successful, then you will have an executable named `marbleSphereSimulation` in your build directory. Execute it with

`./marbleSphereSimulation`
