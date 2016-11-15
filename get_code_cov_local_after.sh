#!/bin/bash

# For getting the code coverage locally
# 1) Run get_code_cov_local_init.sh
# 2) Run the executable(s)
# 3) Run this script

lcov --no-external --capture --directory . --output-file out.info
genhtml -o webpage out.info
cd webpage
firefox index.html
