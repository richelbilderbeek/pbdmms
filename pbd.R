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