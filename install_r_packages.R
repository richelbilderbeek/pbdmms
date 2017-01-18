# Installs the R packages needed
install.packages("devtools", repos = "http://cran.uk.r-project.org")
devtools::install_github("richelbilderbeek/DAISIE")
devtools::install_github("richelbilderbeek/nLTT")
devtools::install_github("rsetienne/PBD", subdir = "PBD")
warnings()
