# pbdmms

Branch|[![Travis CI logo](TravisCI.png)](https://travis-ci.org)|[![Codecov logo](Codecov.png)](https://www.codecov.io)
---|---|---
master|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=master)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=master)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/master)
daic|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=daic)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=daic)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/daic)
elly|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=elly)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=elly)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/elly)
jaan|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=jaan)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=jaan)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/jaan)
jobo|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=jobo)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=jobo)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/jobo)
kewe|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=kewe)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=kewe)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/kewe)
pbd|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=pbd)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=pbd)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/pbd)
ribi|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=ribi)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=ribi)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/ribi)
sado|[![Build Status](https://travis-ci.org/richelbilderbeek/pbdmms.svg?branch=sado)](https://travis-ci.org/richelbilderbeek/pbdmms)|[![codecov.io](https://codecov.io/github/richelbilderbeek/pbdmms/coverage.svg?branch=sado)](https://codecov.io/github/richelbilderbeek/pbdmms/branch/sado)

## Build status of builds that `pbdmms` relies on

 * [![Build Status](https://travis-ci.org/richelbilderbeek/BoostGraphTutorial.svg?branch=master)](https://travis-ci.org/richelbilderbeek/BoostGraphTutorial) [![codecov.io](https://codecov.io/github/richelbilderbeek/BoostGraphTutorial/coverage.svg?branch=master)](https://codecov.io/github/richelbilderbeek/BoostGraphTutorial?branch=master) [BoostGraphTutorial](https://github.com/richelbilderbeek/BoostGraphTutorial)
 * [![Build Status](https://travis-ci.org/Microsoft/GSL.svg?branch=master)](https://travis-ci.org/Microsoft/GSL) [![codecov.io](https://codecov.io/github/Microsoft/GSL/coverage.svg?branch=master)](https://codecov.io/github/Microsoft/GSL?branch=master) [GSL](https://github.com/Microsoft/GSL)
 * [![Build Status](https://travis-ci.org/richelbilderbeek/gausser.svg?branch=master)](https://travis-ci.org/richelbilderbeek/gausser) [![codecov.io](https://codecov.io/github/richelbilderbeek/gausser/coverage.svg?branch=master)](https://codecov.io/github/richelbilderbeek/gausser?branch=master) [gausser](https://github.com/richelbilderbeek/gausser)
 * [![Build Status](https://travis-ci.org/richelbilderbeek/SurfacePlotter.svg?branch=master)](https://travis-ci.org/richelbilderbeek/SurfacePlotter) [![codecov.io](https://codecov.io/github/richelbilderbeek/SurfacePlotter/coverage.svg?branch=master)](https://codecov.io/github/richelbilderbeek/SurfacePlotter?branch=master) [SurfacePlotter](https://github.com/richelbilderbeek/SurfacePlotter)
 * [![Build Status](https://travis-ci.org/richelbilderbeek/RibiLibraries.svg?branch=master)](https://travis-ci.org/richelbilderbeek/RibiLibraries) [![codecov.io](https://codecov.io/github/richelbilderbeek/RibiLibraries/coverage.svg?branch=master)](https://codecov.io/github/richelbilderbeek/RibiLibraries?branch=master) [RibiLibraries](https://github.com/richelbilderbeek/RibiLibraries)
 * [![Build Status](https://travis-ci.org/richelbilderbeek/RibiUnits.svg?branch=master)](https://travis-ci.org/richelbilderbeek/RibiUnits) [![codecov.io](https://codecov.io/github/richelbilderbeek/RibiUnits/coverage.svg?branch=master)](https://codecov.io/github/richelbilderbeek/RibiUnits?branch=master) [RibiUnits](https://github.com/richelbilderbeek/RibiUnits)

## Screenshots

See [Screenshots](Screenshots/README.md)

## Setup

For each namespace `x`, there is:
 * `x.pro`
 * `x_gui.pro`
 * `x_test.pro`

The purpose of each of these projects is:

 * `x.pro`: compiled in release mode and profiled by `gprof`. Tip: setup a default setting that profiling takes between one and ten minutes
 * `x_gui.pro`: visualizes the simulation
 * `x_test.pro`: compiled in debug mode (with `gcov` and `UBSAN`) to test all code with Boost.Test and measure code coverage

Each namespace `x` also has a git branch with that 
name ([what is git again?](Documentation/git.md))

## git and GitHub

See [the git and GitHub documentation](Documentation/git.md)

## Installation

See [the installation documentation](Documentation/install.md)

## Building

See [the build documentation](Documentation/build.md)

## Code of conduct

The repository adheres to the [Contributor Covenant Code of Conduct](code_of_conduct.md).

## Contributing, code guidelines

See [CONTRIBUTING](CONTRIBUTING.md).

## Schedule

See [Schedule](Schedule.md)

## References

 * [Etienne & Rosindell 2012] Etienne, Rampal S., and James Rosindell. "Prolonging the past counteracts the pull of the present: protracted speciation can explain observed slowdowns in diversification." Systematic Biology (2012): syr091.
 * [Eteinne et al., 2014] Etienne, Rampal S., Helene Morlon, and Amaury Lambert. "Estimating the duration of speciation from phylogenies." Evolution 68.8 (2014): 2430-2440.
 * [Janzen et al., 2015] Janzen, Thijs, Sebastian Höhna, and Rampal S. Etienne. "Approximate Bayesian computation of diversification rates from molecular phylogenies: introducing a new efficient summary statistic, the nLTT." Methods in Ecology and Evolution 6.5 (2015): 566-575.
 * [Van Doorn et al., 1998] Van Doorn, G. S., A. J. Noest, and P. Hogeweg. "Sympatric speciation and extinction driven by environment dependent sexual selection." Proceedings of the Royal Society of London B: Biological Sciences 265.1408 (1998): 1915-1919.
 * [Van Doorn et al., 2001] Van Doorn, G. Sander, Pieternella C. Luttikhuizen, and Franz J. Weissing. "Sexual selection at the protein level drives the extraordinary divergence of sex–related genes during sympatric speciation." Proceedings of the Royal Society of London B: Biological Sciences 268.1481 (2001): 2155-2161.
 * [Van Doorn et al., 2002] van Doorn, G. Sander, and Franz J. Weissing. "Ecological versus sexual selection models of sympatric speciation: a synthesis." Selection 2.1-2 (2002): 17-40.
 * [Weissing et al., 2011] Weissing, Franz J., Pim Edelaar, and G. Sander Van Doorn. "Adaptive speciation theory: a conceptual review." Behavioral ecology and sociobiology 65.3 (2011): 461-480.
 * [Wilson et al., 2014] Wilson, Greg, et al. "Best practices for scientific computing." PLoS Biol 12.1 (2014): e1001745.
 * and more...

## Portfolio's

Personal portfilio's, thus put in private repositories:

 * :lock: [elly_portfolio](https://github.com/richelbilderbeek/elly_portfolio)
 * :lock: [jobo_portfolio](https://github.com/richelbilderbeek/jobo_portfolio)
 * :lock: [kewe_portfolio](https://github.com/richelbilderbeek/kewe_portfolio)

## Articles in preparation

The article(s)-in-progress are put in private repositories:

 * :lock: [distancer_article](https://github.com/richelbilderbeek/distancer_article): about the `ribi` model
 * :lock: [wirittte_article](https://github.com/richelbilderbeek/wirittte_article): richelbilderbeek's other project

