# Installs the R packages needed
install.packages("devtools", repos = "http://cran.uk.r-project.org")

# v1.2, does not support Endemic&NonEndemic yet
# install.packages("DAISIE", repos = "http://cran.uk.r-project.org")
# v1.3, does support Endemic&NonEndemic, but is closed
devtools::install_github("richelbilderbeek/DAISIE_1_3", subdir = "DAISIE")

devtools::install_github("richelbilderbeek/nLTT")
devtools::install_github("rsetienne/PBD", subdir = "PBD")
warnings()

#packageVersion("DAISIE")
packageVersion("devtools")
packageVersion("nLTT")
packageVersion("PBD")
