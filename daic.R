time <- 100.0
mainland_n <- 1
lac <- 10.2
mu <- 10.5
k <- 50
gam <- 10.1
laa <- 10.3
pars <- c(lac, mu, k, gam, laa)
results <- DAISIE::DAISIE_sim_core(time, mainland_n, pars)
names(results)
results$species_table
results$taxon_list
results
