#!/bin/bash

OUTPUT="main"

SOURCE="main.cpp"

LIBRARIES="-lpugixml -lps4000a"

LIB_PATH="-L/opt/picoscope/lib/"

OTHER_FILES="picofunctions.cpp"

g++ -o $OUTPUT $SOURCE $OTHER_FILES $LIB_PATH $LIBRARIES

if [ $? -eq 0 ]; then
    echo "Compilation successful."
else
    echo "Compilation failed."
    exit 1
fi