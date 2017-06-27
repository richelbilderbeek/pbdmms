library(ape)
library(PBD)
sink(file = tempfile())
ml <- PBD::pbd_ML(
  brts = ape::branching.times(
    read.tree(text = "(:10);")),
  initparsopt = c(0.2,0.01,0.3),
  exteq = 1 # mu_1 == mu_2
)
sink()
write.csv(ml, "calc_max_likelihood.txt")
