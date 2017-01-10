library(DAISIE)
# data(Galapagos_datatable)
# write.csv(
#  x = Galapagos_datatable, 
#  file = "~/elly_daisie_input.csv", 
#  row.names = FALSE)
setwd("~")
getwd()

filename_fail <- "elly_daisie_input_reality.csv"
filename_ok <- "elly_daisie_input_ideal.csv"
filename_fixed <- "elly_daisie_input_reality.csv"

df_ok <- read.csv(file = filename_ok, sep = '\t')
df_fail <- read.csv(file = filename_fail, sep = '\t')

island_age <- 10.0
n_species_main <- 1000
prepared_df_ok <- DAISIE_dataprep(
  datatable = df_ok,
  island_age = island_age,
  M = n_species_main)
prepared_df_fail <- DAISIE_dataprep(
  datatable = df_fail,
  island_age = island_age,
  M = n_species_main)



output_ok <- DAISIE_ML(
   datalist = prepared_df_ok,
   initparsopt = c(2.5,2.7,20,0.009,1.01),
   ddmodel = 11,
   idparsopt = 1:5,
   parsfix = NULL,
   idparsfix = NULL
)
output_fail <- DAISIE_ML(
   datalist = prepared_df_fail,
   initparsopt = c(2.5,2.7,20,0.009,1.01),
   ddmodel = 11,
   idparsopt = 1:5,
   parsfix = NULL,
   idparsfix = NULL
)

write.csv(
  x = output, 
  file = "~/elly_daisie_output.csv", 
  sep = " ",
  row.names = FALSE
)


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

DAISIE_tutorial()


Clade_name  Status	Missing_species	Branching_times
33	Non_endemic	0	0