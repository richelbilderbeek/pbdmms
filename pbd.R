# For GUI
create_plot <- function() {

  library(PBD)

  birth_good <- 0.2
  completion <- 0.2
  birth_incipient <- 0.2
  death_good <- 0.1
  death_incipient <- 0.1
  age  <- 15
  filename <- "pbd.png"

  png(filename)
  PBD::pbd_sim(
    pars = c(
      birth_good,
      completion,
      birth_incipient,
      death_good,
      death_incipient
    ),
    age = age,
    soc = 2,
    plotit = TRUE
  )
  dev.off()

}

library(PBD)
filename <- "pbd.csv"
birth_good <- 0.2
completion <- 0.2
birth_incipient <- 0.2
death_good <- 0.1
death_incipient <- 0.1
age  <- 15

out <- PBD::pbd_sim(
  pars = c(
    birth_good,
    completion,
    birth_incipient,
    death_good,
    death_incipient
  ),
  age = age,
  soc = 2
)
write.csv(x = out$L, file = filename)






library(PBD)
filename <- "nltt_recontree.csv"
birth_good <- 0.2
completion <- 0.2
birth_incipient <- 0.2
death_good <- 0.1
death_incipient <- 0.1
age  <- 15

out <- PBD::pbd_sim(
  pars = c(
    birth_good,
    completion,
    birth_incipient,
    death_good,
    death_incipient
  ),
  age = age,
  soc = 2
)

phy <- out$recontree
#we use the ltt.plot.coords function from the package ape
xy <- ape::ltt.plot.coords( phy, backward = TRUE, tol = 1e-6)
xy[, 2] <- xy[, 2] / max(xy[, 2]) #normalize number lineages

xy[, 1] <- xy[, 1] + abs( min( xy[, 1])) #make sure time runs from 0..T
xy[, 1] <- xy[, 1] / max( xy[, 1])      #normalize time

write.csv(x = xy, file = filename)
