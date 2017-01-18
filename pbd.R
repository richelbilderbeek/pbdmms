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


create_l_table() {
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
}

create_nltt <- function() {

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
}



# calc_nltt_statistic




brts <- branching.times(phylogeny)  # branching times
init_b <- 0.2  # speciation-initiation rate
init_mu_1 <- 0.05  # extinction rate of good species
init_la_1 <- 0.3 # speciation-completion rate
#init_mu_2 <- 0.05  # extinction rate of incipient species  # not used

# The initial values of the parameters that must be optimized
initparsopt <- c(init_b, init_mu_1, init_la_1)

# The extinction rates between incipient and good species are equal
exteq <- TRUE

# The first element of the branching times is the crown age (and not the stem age)
soc <- 2

# Conditioning on non-extinction of the phylogeny
# as I actively selected for a nice phylogeny
cond <- 1

# Give the likelihood of the phylogeny (instead of the likelihood of the branching times)
btorph <- 1
```

Calling the `pbd_ML` function will give a lot of noise. 
It can be turned off by setting verbose = FALSE.
Here goes:

```{r}
r <- pbd_ML(
  brts = brts,
  initparsopt = initparsopt, 
  exteq = exteq,
  soc = soc, 
  cond = cond,
  btorph = btorph
)
```

The ML parameter estimates are:

```{r}
print(r)


