library(PBD)
set.seed(20)
filename <- "jobo_pbd_nltt.csv"
birth_good <- 0.5
completion <- 0.3
birth_incipient <- 0.3
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

phy <- out$igtree.extinct
xy <- ape::ltt.plot.coords( phy, backward = TRUE, tol = 1e-6)
xy[, 2] <- xy[, 2] / max(xy[, 2])
xy[, 1] <- xy[, 1] + abs( min( xy[, 1]))
xy[, 1] <- xy[, 1] / max( xy[, 1])

write.csv(x = xy, file = filename)
