library(DAISIE)
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
?DAISIE
DAISIE_tutorial()


# data(Galapagos_datatable)
# write.csv(
#  x = Galapagos_datatable, 
#  file = "~/elly_daisie_input.csv", 
#  row.names = FALSE)

df <- read.csv(file = "~/elly_daisie_input_simpler.csv")

prepared_df <- DAISIE_dataprep(
  datatable = df,
  island_age = 4,
  M = 1000)


output <- DAISIE_ML(
   datalist = prepared_df,
   initparsopt = c(2.5,2.7,20,0.009,1.01),
   ddmodel = 11,
   idparsopt = 1:5,
   parsfix = NULL,
   idparsfix = NULL
)
output

#DAISIE_ML(
#   datalist = prepared_df,
#   initparsopt = c(2.5,2.7,0.009,1.01),
#   idparsopt = c(1,2,4,5),
#   parsfix = Inf,
#   idparsfix = 3
#   )

# idparsopt <- 1:5 # Estimate all parameters
# idparsfix <- NULL # No parameters are kept fixed
# initparsopt <- c(2.5,2.7,20,0.009,1.01)
# parsfix <- NULL # No fixed parameter
# ddmodel <- 11 #  Linear dependence in speciation rate and in immigration rate 
# # res <- 30 # Maximum number of species in a clade to be computed
# cond <- 0 # Conditioning has not been implemented and may not make sense. Cond is set to 0
# eqmodel <- 0 # No equilibrium is assumed 
# 
# 
# results <- DAISIE_ML(
#   datalist = prepared_df,
#   initparsopt = initparsopt,
#   idparsopt = idparsopt,
#   parsfix = parsfix,
#   idparsfix = idparsfix,
#   ddmodel = ddmodel,
#   cond = cond,
#   eqmodel = eqmodel,
# )
#   
# 
# ?DAISIE_ML
