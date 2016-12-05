#include "histogram_to_png.h"

#include <iostream>
#include <ctime>

#include <QApplication>

#include "lyke_individual.h"
#include "lyke_random.h"
#include "lyke_simulation.h"
#include "lyke_utils.h"
#include "lyke_parameters.h"

#include "pbd_helper.h"
#include "qtsurfaceplotwidget.h"

std::vector<double> convert_to_vd(const std::vector<std::string>& v)
{
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const std::string& i) {
      return  std::stod(i);
    }
  );
  return w;
}

std::vector<double> convert_to_vd(const std::vector<int>& v)
{
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const int i) { return static_cast<double>(i); }
  );
  return w;
}

std::vector<std::vector<double>> read_histogram_in_time(const std::string& filename)
{
  std::vector<std::string> text = pbd::file_to_vector(filename);
  //Skip the first line, as this is the header
  text.erase(text.begin());

  std::vector<std::vector<double>> v(text.size());
  const int n_lines{static_cast<int>(text.size())};
  for (int i=0; i!=n_lines; ++i)
  {
    std::vector<std::string> words = pbd::seperate_string(text[i], ',');
    //Remove first word, as this is the generation number
    words.erase(words.begin());
    if (words.back() == "") words.pop_back(); //Lines end with a comma :-(
    v[i] = convert_to_vd(words);
  }
  return v;
}


void histogram_to_png(
  const std::string& csv_filename,
  const std::string& png_filename
)
{
  const std::vector<std::vector<double>> histogram_in_time{
    read_histogram_in_time(csv_filename)
  };
  assert(!histogram_in_time.empty());
  assert(!histogram_in_time[0].empty());
  const int height{static_cast<int>(histogram_in_time.size())};
  const int width{static_cast<int>(histogram_in_time[0].size())};
  ribi::QtSurfacePlotWidget w;
  w.setMinimumHeight(height);
  w.setMinimumWidth(width);
  w.SetSurfaceGrey(histogram_in_time);
  QPixmap pixmap(w.size());
  w.render(&pixmap, QPoint(), QRegion(w.frameGeometry()));
  pixmap.save(QString(png_filename.c_str()));
}
