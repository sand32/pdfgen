#!/bin/sh

# Change this to the location of your Qt SDK
#
# This should be the folder containing Qt's bin/, include/, and lib/ folders
Qt=~/dev/lib/Qt-Current/gcc_64

# Build *nix makefiles
cmake -G "Unix Makefiles" -DQT_LOCATION:PATH=$Qt ../

