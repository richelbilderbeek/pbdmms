# Installs the R packages needed
my_repo <- "http://cran.us.r-project.org"

install.packages("devtools", repos = my_repo)

# v1.2, does not support Endemic&NonEndemic yet
install.packages("DAISIE", repos = my_repo)
# v1.3, does support Endemic&NonEndemic, but is closed
# devtools::install_github("richelbilderbeek/DAISIE_1_3", subdir = "DAISIE")

devtools::install_github("richelbilderbeek/nLTT")
devtools::install_github("rsetienne/PBD", subdir = "PBD")
warnings()

#packageVersion("DAISIE")
packageVersion("devtools")
packageVersion("nLTT")
packageVersion("PBD")
