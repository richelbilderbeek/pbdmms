library(DAISIE)
di_filename <- "experiment_daisie_input_reality.csv"
if(!file.exists(di_filename)) {
  stop("Input file '",di_filename,"' absent")
}
df <- read.csv(file = di_filename, sep = '	')
print(df)
if(nrow(df) == 0) {
  stop("Input must have rows")
}
prepared_df <- DAISIE_dataprep(
  datatable = df,
  island_age = 15,
  M = 1000)
output <- DAISIE_ML(
   datalist = prepared_df,
   initparsopt = c(0.2,0.09,15,0.002,0.87),
   ddmodel = 11,
   idparsopt = 1:5,
   parsfix = NULL,
   idparsfix = NULL
)
write.csv(
  x = output,
  file = "experiment_daisie_output_reality.csv",
  row.names = FALSE
)
