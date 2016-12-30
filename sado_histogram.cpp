#include "sado_histogram.h"

#include "sado_parameters.h"

sado::histogram sado::create_histogram_p(const population& pop, const parameters& params)
{
  const int histw{params.get_histw()};
  const double delta{1.0/static_cast<double>(pop.size())};

  histogram histp(histw, 0.0);
  for(const auto& i: pop)
  {
    const double p{i.get_p()};
    const double histbinp{params.get_histbinp()};
    int index{static_cast<int>(params.get_histw()/2.0+p/histbinp)};
    if(index<0) index=0;
    if(index>=histw) index=histw-1;
    histp[index]+=delta;
  }
  return histp;
}

sado::histogram sado::create_histogram_q(const population& pop, const parameters& p)
{
  const int histw{p.get_histw()};
  const double delta{1.0/static_cast<double>(pop.size())};

  histogram histq(histw, 0.0);
  for(const auto& i: pop)
  {
    const double q{i.get_q()};
    const double histbinq{p.get_histbinq()};
    int index{static_cast<int>(histw/2.0+q/histbinq)};
    if(index<0) index=0;
    if(index>=histw) index=histw-1;
    histq[index]+=delta;
  }
  return histq;
}

sado::histogram sado::create_histogram_x(const population& pop, const parameters& p)
{
  const int histw{p.get_histw()};
  const double delta{1.0/static_cast<double>(pop.size())};

  histogram histx(histw, 0.0);
  for(const auto& i: pop)
  {
    const double x{i.get_x()};
    const double histbinx{p.get_histbinx()};
    int index{static_cast<int>(histw/2.0+x/histbinx)};
    if(index<0) index=0;
    if(index>=histw) index=histw-1;
    histx[index]+=delta;
  }
  return histx;

}

