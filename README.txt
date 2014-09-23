==== SequenceTree4 ====

COMPILATION INSTRUCTIONS for Qt4 with MinGW on Windows

  0. Be sure you have Qt4 and MinGW installed properly
  1. Extract to a folder with no spaces, e.g. c:\st4
  2. Open a command window (in the Qt4/MinGW environment)
  3. cd to the "src� directory
  4. Run qmake
  5. Run mingw32-make
  6. Executable will be created in the �bin� directory

COMPILATION INSTRUCTIONS for Linux

  0. Be sure you have Qt4 installed properly
  1. cd src
  2. qmake
  3. make
  4. Executable will be created in the �bin� directory
  5. Before running the program set the following environment variable:
	export LD_LIBRARY_PATH=[st4-directory]/bin


