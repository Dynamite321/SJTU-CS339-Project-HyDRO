All the *.c files are in the folder "../src", and all the *.h files are in the folder "../include".

How to modify parameters:
All the parameters are in header file "..include/parameter.h".

How to make:
rm -rf build
mkdir build && cd build
cmake ..
make

We have already made the source code, and you can directly run it if you do not want to adjust the parameters. Time of simulation is about 20 min, you can adjust it in "../include/parameters.h".

How to run:
cd build
./main

All the intermediate results are available in "../build/out.txt"
