#!/bin/bash

git_branch=`git rev-parse --abbrev-ref HEAD`
echo $git_branch
cpp_files=`ls {ribi_,pbd_}*.cpp | egrep -v "^qrc_.*\.cpp$" | egrep -v "^moc_.*\.cpp$" | egrep -v "^.*_test\.cpp$"`
echo ".cpp files that will be analysed: "$cpp_files
h_files=`ls ribi_*.h | egrep -v "^ui_.*\.h$"`
echo ".h files that will be analysed: "$h_files

qt4_folder="usr/include/qt4/QtGui"
qt5_folder="usr/include/qt5/QtWidgets"

./oclint-0.10.3/bin/oclint -o oclint.log \
  -disable-rule ShortVariableName \
  $cpp_files \
  $h_files \
  -- \
  -c -std=c++14 -fPIE \
  -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB \
  -I../BoostGraphTutorial/BoostGraphTutorial \
  -I../RibiLibraries/qwt-6.1/src \
  -I../SurfacePlotter \
  -I/usr/include/c++/5 \
  -I/usr/include/x86_64-linux-gnu/c++/5 \
  -I/usr/include/qt5 \
  -I/usr/include/qt5/QtCore \
  -I/usr/include/qt5/QtGui \
  -I/usr/include/qt5/QtTest \
  -I/usr/include/qt5/QtWidgets 


cat oclint.log

# Will be 0 if success
# Will be 1 if fail
fail=`egrep "Compiler Errors" oclint.log | wc -l`

if [ $fail -eq 1 ]; 
then
  echo "OCLint: Compiler error, can only be warnings :-("
else
  echo "OCLint: OK"
fi

# Will be 1 if success
# Will be 0 if fail
success=`egrep "FilesWithViolations=0 P1=0 P2=0 P3=0" oclint.log | wc -l`

if [ $success -eq 1 ]; 
then
  echo "OCLint: OK"
else
  echo "OCLint: Fail"
  exit 1
fi
