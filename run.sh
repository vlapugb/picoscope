#!/bin/bash

OUTPUT="main"

SOURCE="main.cpp"

LIBRARIES="-lpugixml -lps4000a"

LIB_PATH="-L/opt/picoscope/lib/"

g++ -o $OUTPUT $SOURCE $LIB_PATH $LIBRARIES

if [ $? -eq 0 ]; then
    echo "Compilation successful."
else
    echo "Compilation failed."
    exit 1
fi