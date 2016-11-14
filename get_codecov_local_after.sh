lcov --no-external --capture --directory . --output-file out.info
genhtml -o webpage out.info
cd webpage
firefox index.html
