#!/bin/bash


# Normal
cd src_NormalAndArcadeMode

make clean
make -f Makefile16MHz clean

make
cp *.hex ../
make clean

make -f Makefile16MHz
cp *.hex ../
make -f Makefile16MHz clean

cd ..

# Arcade
cd src_NormalAndArcadeMode/ArcadeMode

make clean
make -f Makefile16MHz clean

make
cp *.hex ../../
make clean

make -f Makefile16MHz
cp *.hex ../../
make -f Makefile16MHz clean

cd ../..

# Xbox
cd src_XboxMode

make clean
make -f Makefile16MHz clean

make
cp *.hex ../
make clean

make -f Makefile16MHz
cp *.hex ../
make -f Makefile16MHz clean

cd ..

# Switch
cd src_SwitchMode

make clean
make -f Makefile16MHz clean

make
cp *.hex ../
make clean

make -f Makefile16MHz
cp *.hex ../
make -f Makefile16MHz clean

cd ..

# MiSTer
cd src_MiSTerMode

make clean
make -f Makefile16MHz clean

make
cp *.hex ../
make clean

make -f Makefile16MHz
cp *.hex ../
make -f Makefile16MHz clean

cd ..

# Undamned
cd src_UndamnedMode

make clean
make -f Makefile16MHz clean

make
cp *.hex ../
make clean

make -f Makefile16MHz
cp *.hex ../
make -f Makefile16MHz clean

cd ..
