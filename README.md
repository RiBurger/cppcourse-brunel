# cppcourse-brunel

This repository contains all my work from the last three weeks of project. My work from the previous weeks are in my initial repository (it was asked to change in the middle of the project), which is named *CS116-2017-eric-bergkvist*. So you can see there that I've produced a continuous work throughout the whole project, and I simply don't know how to merge my two repositories.

To run the program, you have to put the following files in a document :
- Neuron.h
- Neuron.cpp
- Network.h
- Network.cpp
- Main.cpp
- Neuron_unittest.cpp
- Doxyfile.in
- CMakeLists.txt
- The gtest document from the googletest GitHub, and make sure it's named "googletest".

Then open the terminal, go to the directory of the document, and enter "cmake ."
Then, enter "make". This will create a "Neurons" executable, and you can run the program by entering "./Neurons".
It will also create a "Neuron_unittest" executable, and you can run the tests by entering "./Neuron_unittest".

The NeuronGraphi.txt files, with i going from 1 to 4, are the data used to reproduce the Brunel figures (p.197 of his paper), which you can find in the "Plots.pdf" file.
