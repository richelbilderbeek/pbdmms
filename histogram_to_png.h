#ifndef HISTOGRAM_TO_PNG_H
#define HISTOGRAM_TO_PNG_H

#include <string>

///Converts a histogram-like saved file,
///as used by van Doorn to a PNG
void histogram_to_png(
  const std::string& csv_filename,
  const std::string& png_filename
);


#endif // HISTOGRAM_TO_PNG_H
