# pbdmms

Branch|[![Travis CI logo](TravisCI.png)](https://travis-ci.org)|[![Codecov logo](Codecov.png)](https://www.codecov.io)
---|---|---
master|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=master)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=master)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/master)
cine|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=cine)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=cine)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/cine)
elly|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=elly)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=elly)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/elly)
jaan|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=jaan)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=jaan)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/jaan)
jobo|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=jobo)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=jobo)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/jobo)
kewe|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=kewe)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=kewe)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/kewe)
lyke|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=lyke)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=lyke)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/lyke)
ribi|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=ribi)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=ribi)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/ribi)

## Screenshots

![kewe](Screenshots/kewe_gui.png)

![lyke](Screenshots/lyke_gui.png)

![pbd](Screenshots/pbd_gui.png)

![ribi](Screenshots/ribi_gui.png)

## Setup

For each namespace `x`, there is:
 * `x.pro`
 * `x_gui.pro`
 * `x_test.pro`

The purpose of each of these projects is:

 * `x.pro`: compiled in release mode and profiled by `gprof`. Tip: setup a default setting that profiling takes between one and ten minutes
 * `x_gui.pro`: visualizes the simulation
 * `x_test.pro`: compiled in debug mode (with `gcov` and `UBSAN`) to test all code with Boost.Test and measure code coverage

## Installation on LWP

 * `apt-get install graphviz qtcreator`: install `make`, Qt Creator and GraphViz
 * `git clone https://github.com/richelbilderbeek/pbdmms`: clone this repository
 * `cd pbdmms`: move into the folder `pbdmms`
 * `./checkout_all_branches.sh`: checkout all `git` branches
 * `./download_other_githubs.sh`: download all other GitHubs needed
 * Optional: `git checkout x`: checkout branch of namespace `x`, where `x` can be any of the branch names

## Installation on Peregrine

 * `module load git`: allow the use of `git`
 * `git clone https://github.com/richelbilderbeek/pbdmms`: clone this repository
 * `cd pbdmms`: move into the folder `pbdmms`
 * `./checkout_all_branches.sh`: checkout all `git` branches
 * `./download_other_githubs.sh`: download all other GitHubs needed
 * Optional: `git checkout x`: checkout branch of namespace `x`, where `x` can be any of the branch names

## Building on LWP

Either load the `.pro` file from Qt Creator, or use `qmake` to build:

 * `qmake my.pro` or `qmake -qt=qt5 x.pro`: creates a makefile for project `x`
 * `make` or `make debug` or `make release`: build that makefile in debug, debug or release mode respectively
 * `./x`: start the created executable `x`

## Building on Peregrine

Either load the `.pro` file from Qt Creator, or use `qmake` to build:

 * `module load GCC/5.1.0`: load g++ version 5.1.0
 * `module load Qt`: load `qmake`
 * `qmake my.pro` or `qmake -qt=qt5 x.pro`: creates a makefile for project `x`
 * `make` or `make debug` or `make release`: build that makefile in debug, debug or release mode respectively
 * `./x`: start the created executable `x`

## Contributing, code guidelines

See [CONTRIBUTING.md](CONTRIBUTING.md).

## Schedule

 * 2016-09-05: First day `jobo` and `kewe`
 * 2016-11-17 9:30 Progress meeting `jobo`, `kewe`, `ribi` and Rampal
 * 2016-11-17: Midterm progress meeting `jobo`
 * 2016-12-12: First full version of BDM-model `jobo`
 * 2016-12-14 9:30 TECE meeting
 * 2017-01-03: ?Midterm progress meeting `kewe`
 * 2017-01-?? 9:30 TECE meeting
 * 2017-02-10: Final day `jobo`
 * 2017-?03-??: Final day `kewe`

## Articles used

 * Etienne, Rampal S., and James Rosindell. "Prolonging the past counteracts the pull of the present: protracted speciation can explain observed slowdowns in diversification." Systematic Biology (2012): syr091.
 * Etienne, Rampal S., Helene Morlon, and Amaury Lambert. "Estimating the duration of speciation from phylogenies." Evolution 68.8 (2014): 2430-2440.
 * van Doorn, G. Sander, and Franz J. Weissing. "Ecological versus sexual selection models of sympatric speciation: a synthesis." Selection 2.1-2 (2002): 17-40.
 * Weissing, Franz J., Pim Edelaar, and G. Sander Van Doorn. "Adaptive speciation theory: a conceptual review." Behavioral ecology and sociobiology 65.3 (2011): 461-480.
 * and more...

## Portfolio's

Personal portfilio's, thus put in private repositories:

 * :lock: [jobo_portfolio](https://github.com/richelbilderbeek/jobo_portfolio)
 * :lock: [kewe_portfolio](https://github.com/richelbilderbeek/kewe_portfolio)

## Articles in preparation

The article(s)-in-progress are put in private repositories:

 * :lock: [ribi](https://github.com/richelbilderbeek/distancer_article)

