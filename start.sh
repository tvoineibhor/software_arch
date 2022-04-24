#!/bin/bash

HOST=127.0.0.1
PORT=6033
LOGIN=stud
PASSWORD=stud
DATABASE=stud

BUILD_DIR=build
EXECUTABLE_FILE=$BUILD_DIR/poco-http-server-lab2

mkdir -p build

cd build
cmake ..
make

cd ..

./$EXECUTABLE_FILE \
    --host=$HOST \
    --port=$PORT \
    --login=$LOGIN \
    --password=$PASSWORD \
    --database=$DATABASE
