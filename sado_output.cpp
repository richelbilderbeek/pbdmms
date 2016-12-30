#include "sado_output.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include "sado_helper.h"
#include "sado_parameters.h"
#include "sado_results.h"

void sado::append_histogram(
  const histogram& p,
  const std::string& filename
)
{
  assert(!p.empty());
  const double m{
    *std::max_element(
      std::begin(p),
      std::end(p)
    )
  };
  assert(m != 0.0);

  std::stringstream s;
  for (double d: p)
  {
    s << (d / m) << ',';
  }
  std::string t{s.str()};
  assert(!t.empty());
  t.resize(t.size() - 1);

  std::ofstream f(filename, std::ios_base::app);
  f << t << '\n';
}

void sado::output(
  const population& pop,
  const int t,
  const parameters& p,
  results& r
)
{
  const int pop_size{static_cast<int>(pop.size())};
  double ssxx=0.0,ssxp=0.0,sspp=0.0,ssxq=0.0,ssqq=0.0,sspq=0.0,
      maxx=0.0,maxp=0.0,maxq=0.0;

  const double delta{1.0/pop_size};
  const int histw{p.get_histw()};
  histogram histx(histw, 0.0);
  histogram histp(histw, 0.0);
  histogram histq(histw, 0.0);
  const double avgx{get_mean_x(pop)};
  const double avgp{get_mean_p(pop)};
  const double avgq{get_mean_q(pop)};
  for(auto i=std::cbegin(pop);i!=std::cend(pop);i++)
  {
    const double xi{i->get_x()};
    const double pi{i->get_p()};
    const double qi{i->get_q()};
    const double dxi{xi-avgx};
    const double dpi{pi-avgp};
    const double dqi{qi-avgq};
    ssxx+=dxi*dxi;
    ssxp+=dxi*dpi;
    ssxq+=dxi*dqi;
    sspp+=dpi*dpi;
    sspq+=dpi*dqi;
    ssqq+=dqi*dqi;
    const double histbinp{p.get_histbinp()};
    const double histbinq{p.get_histbinq()};
    const double histbinx{p.get_histbinx()};
    int jx{static_cast<int>(histw/2.0+xi/histbinx)};
    int jp{static_cast<int>(histw/2.0+pi/histbinp)};
    int jq{static_cast<int>(histw/2.0+qi/histbinq)};
    if(jx<0) jx=0;
    if(jx>=histw) jx=histw-1;
    if(jp<0) jp=0;
    if(jp>=histw) jp=histw-1;
    if(jq<0) jq=0;
    if(jq>=histw) jq=histw-1;
    histx[jx]+=delta;
    if(histx[jx]>maxx) maxx=histx[jx];
    histp[jp]+=delta;
    if(histp[jp]>maxp) maxp=histp[jp];
    histq[jq]+=delta;
    if(histq[jq]>maxq) maxq=histq[jq];

  }
  const double rhoxp{ssxp/std::sqrt(ssxx*sspp)};
  const double rhoxq{ssxq/std::sqrt(ssxx*ssqq)};
  const double rhopq{sspq/std::sqrt(sspp*ssqq)};
  const double sx{std::sqrt(ssxx/(pop_size-1.0))};
  const double sp{std::sqrt(sspp/(pop_size-1.0))};
  const double sq{std::sqrt(ssqq/(pop_size-1.0))};

  std::ofstream out(p.get_output_filename());
  std::stringstream s;
  s  <<t<<","<<pop_size<<","<<rhoxp<<","<<rhoxq<<","<<rhopq<<","<<sx<<","<<sp<<","<<sq;
  out<<t<<","<<pop_size<<","<<rhoxp<<","<<rhoxq<<","<<rhopq<<","<<sx<<","<<sp<<","<<sq;
  std::cout<<t<<" "<<pop_size<<" "<<rhoxp<<" "<<rhoxq<<" "<<rhopq<<'\n'
     <<avgx<<" "<<avgp<<" "<<avgq<<" "<<sx<<" "<<sp<<" "<<sq<<'\n';

  {
    r.m_ecological_trait.push_back(histx);
    r.m_female_preference.push_back(histp);
    r.m_male_trait.push_back(histq);
    r.m_rhopq.push_back(rhopq);
    r.m_rhoxp.push_back(rhoxp);
    r.m_rhoxq.push_back(rhoxq);
    r.m_sp.push_back(sp);
    r.m_sq.push_back(sq);
    r.m_sx.push_back(sx);
    r.m_t.push_back(t);
    append_histogram(histx, "eco_traits.csv");
    append_histogram(histp, "fem_prefs.csv");
    append_histogram(histq, "male_traits.csv");
  }
  for(int j=0;j<histw;j++)
  {
    out<<","<<histx[j]/maxx;
    s  <<","<<histx[j]/maxx;
  }
  for(int j=0;j<histw;j++)
  {
    out<<","<<histp[j]/maxp;
    s  <<","<<histp[j]/maxp;
  }
  for(int j=0;j<histw;j++)
  {
    out<<","<<histq[j]/maxq;
    s  <<","<<histq[j]/maxq;
  }
  out<<'\n';
  if (is_golden_standard(p))
  {
    try
    {
      const std::string golden{get_golden_output().at( (t / 10) + 1)};
      const std::string measured{s.str()};
      const histogram golden_values{
        to_doubles(seperate_string(golden, ','))
      };
      const histogram measured_values{
        to_doubles(seperate_string(measured, ','))
      };
      std::clog << "Comparing:\n"
        << "golden  : " << golden << '\n'
        << "measured: " << measured << '\n'
      ;
      assert(is_more_or_less_same(golden_values, measured_values));
    }
    catch (std::exception&)
    {
      //OK, is beyond golden output
    }
  }
}
