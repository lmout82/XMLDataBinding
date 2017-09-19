#! /bin/bash

if g++ -g ./demo/demo.cpp ./src/*.cpp -o ./build/demo; then
    ./build/demo
fi
