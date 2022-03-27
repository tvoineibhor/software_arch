#!/bin/bash

HOST=localhost
PORT=3306
LOGIN=stud
PASSWORD=stud
DATABASE=stud

BUILD_DIR=build
EXECUTABLE_FILE=$BUILD_DIR/poco-http-server-lab1

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
