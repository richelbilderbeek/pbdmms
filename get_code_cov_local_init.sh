#!/bin/bash

# For getting the code coverage locally
# 1) Run this script
# 2) Run the executable(s)
# 3) Run get_code_cov_local_after.sh

lcov --no-external --capture --initial --directory . --output-file out.info
