df <- read.csv(
  "lyke_histogram_R_new", 
  header=TRUE, 
  sep=",", 
  quote="\"", 
  dec=".", 
  fill=TRUE
)

filename <- "lyke_histogram_R_new.csv"
testit::assert(file.exists(filename))
df <- read.csv(filename, header = FALSE)
names(df)
head(df)


library(ggplot2)
df2 <- duplya

p <- ggplot(df2, aes(x=x,y=y))
p <- p + geom_tile(aes(fill=z))
