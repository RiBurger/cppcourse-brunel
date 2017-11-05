# cppcourse-brunel

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
