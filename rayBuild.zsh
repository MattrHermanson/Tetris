#!/bin/zsh

echo Premaking
cd build
./premake5.osx gmake2

echo Building
cd ..
make
