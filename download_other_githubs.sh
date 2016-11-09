#!/bin/bash

cd ..

if [ ! -d BoostGraphTutorial ]; then
 git clone https://github.com/richelbilderbeek/BoostGraphTutorial
fi

if [ ! -d SurfacePlotter ]; then
 git clone https://github.com/richelbilderbeek/SurfacePlotter
fi

if [ ! -d RibiUnits ]; then
 git clone https://github.com/richelbilderbeek/RibiUnits
fi
