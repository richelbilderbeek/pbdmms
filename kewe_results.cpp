#include <fstream>
#include <cmath>
#include <cassert>
#include <iostream>
#include <QFile>

#include "kewe_results.h"
#include "kewe_parameters.h"

genotypes calc_average_genotype(const std::vector<indiv>& pop)
{
  genotypes averages;

  for(auto i=std::begin(pop);i!=std::end(pop);i++)
    {

      averages.m_x+=i->_x();
      averages.m_p+=i->_p();
      averages.m_q+=i->_q();

    }
    averages.m_x/=static_cast<double>(pop.size());
    averages.m_p/=static_cast<double>(pop.size());
    averages.m_q/=static_cast<double>(pop.size());

    return averages;
}

void calculate_rho(
    const std::vector<indiv>& pop,
    const genotypes& averageGenotypes,
    result_variables& result
    )
{

  double ssxx{0.0};
  double ssxp{0.0};
  double ssxq{0.0};
  double sspp{0.0};
  double sspq{0.0};
  double ssqq{0.0};

  for(auto i=std::begin(pop);i!=std::end(pop);i++)
  {
    double dxi=i->_x()-averageGenotypes.m_x;
    double dpi=i->_p()-averageGenotypes.m_p;
    double dqi=i->_q()-averageGenotypes.m_q;

    std::cout << i->_x() << '\n';



    ssxx+=dxi*dxi;
    ssxp+=dxi*dpi;
    ssxq+=dxi*dqi;
    sspp+=dpi*dpi;
    sspq+=dpi*dqi;
    ssqq+=dqi*dqi;


  }

  result.m_rhoxp.push_back(ssxp/sqrt(ssxx*sspp));
  result.m_rhoxq.push_back(ssxq/sqrt(ssxx*ssqq));
  result.m_rhopq.push_back(sspq/sqrt(sspp*ssqq));
}

void calculate_s(
    const std::vector<indiv>& pop,
    const genotypes& averageGenotypes,
    result_variables& result
    )
{
  double ssxx{0.0};
  double ssxp{0.0};
  double ssxq{0.0};
  double sspp{0.0};
  double sspq{0.0};
  double ssqq{0.0};

  for(auto i=std::begin(pop);i!=std::end(pop);i++)
  {
    double dxi=i->_x()-averageGenotypes.m_x;
    double dpi=i->_p()-averageGenotypes.m_p;
    double dqi=i->_q()-averageGenotypes.m_q;

    ssxx+=dxi*dxi;
    ssxp+=dxi*dpi;
    ssxq+=dxi*dqi;
    sspp+=dpi*dpi;
    sspq+=dpi*dqi;
    ssqq+=dqi*dqi;
  }

  assert(pop.size() > 1);



  result.m_sx.push_back(sqrt(ssxx/(static_cast<double>(pop.size())-1.0)));
  result.m_sp.push_back(sqrt(sspp/(static_cast<double>(pop.size())-1.0)));
  result.m_sq.push_back(sqrt(ssqq/(static_cast<double>(pop.size())-1.0)));
}

void output(bigint t,
            std::vector<std::vector<double>> &histX,
            std::vector<std::vector<double>> &histP,
            std::vector<std::vector<double>> &histQ,
            const kewe_parameters& parameters,
            const std::vector<indiv>& pop,
            result_variables& result
            )
{
  result.m_t.push_back(t);
  result.m_popsize.push_back(static_cast<double>(pop.size()));


  std::ofstream out(parameters.output_parameters.outputfilename);
  const int histw = parameters.output_parameters.histw;

  int j,jx,jp,jq;

  const double delta=1.0/static_cast<double>(pop.size());

  std::vector<double> histx(histw, 0.0);
  std::vector<double> histp(histw, 0.0);
  std::vector<double> histq(histw, 0.0);

  genotypes averageGenotypes = calc_average_genotype(pop);
\

  calculate_rho(pop, averageGenotypes, result);
  calculate_s(pop, averageGenotypes, result);

  //assert(result.m_rhoxp.back() >= -1 && result.m_rhoxp.back() <= 1);

  out<<t<<","<<static_cast<double>(pop.size())<<","
     <<result.m_rhoxp.back()<<","<<result.m_rhoxq.back()<<","<<result.m_rhopq.back()<<","
     <<result.m_sx.back()<<","<<result.m_sq.back()<<","<<result.m_sp.back();

  std::cout<<t<<" "<<static_cast<double>(pop.size())<<" "
           <<result.m_rhoxp.back()<<" "<<result.m_rhoxq.back()<<" "<<result.m_rhopq.back()<< std::endl
           <<averageGenotypes.m_x<<" "<<averageGenotypes.m_p<<" "<<averageGenotypes.m_q<<" "
           <<result.m_sx.back()<<" "<<result.m_sq.back()<<" "<<result.m_sp.back()<<std::endl;

  std::vector<double> histXGen;
  std::vector<double> histPGen;
  std::vector<double> histQGen;


  assert(histXGen.empty());
  assert(histPGen.empty());
  assert(histQGen.empty());

  /// normalize output
  double maxx=0.0;
  double maxp=0.0;
  double maxq=0.0;
  for(auto i=std::begin(pop);i!=std::end(pop);i++)
  {
    jx=int(histw/2.0+i->_x()/parameters.output_parameters.histbinx);
    jp=int(histw/2.0+i->_p()/parameters.output_parameters.histbinp);
    jq=int(histw/2.0+i->_q()/parameters.output_parameters.histbinq);

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

  for(j=0;j<histw;j++)
  {
      out<<","<<histx[j]/maxx;
      histXGen.push_back(histx[j]/maxx);
  }

  for(j=0;j<histw;j++)
  {
      out<<","<<histp[j]/maxp;
      histPGen.push_back(histp[j]/maxp);
  }

  for(j=0;j<histw;j++)
  {
      out<<","<<histq[j]/maxq;
      histQGen.push_back(histq[j]/maxq);
  }

  histX.push_back(histXGen);
  histP.push_back(histPGen);
  histQ.push_back(histQGen);

  histXGen.clear();
  histPGen.clear();
  histQGen.clear();

  out<<std::endl;
}

// Count number of borders (from 0 to >0 or from >0 to 0) in a histogram
int countBorders(const std::vector<double> &histogram)
{
    if (histogram.empty()) throw std::invalid_argument("Histogram is empty");

    int size = static_cast<int>(histogram.size());
    int numOfBorders{0};
    for (int i = 0; i<size; ++i)
    {
        double l, r, o = histogram[i];
        if (i==0) l = 0.0;
        else l = histogram[i-1];
        if (i==size-1) r = 0.0;
        else r = histogram[i+1];

        if ((i==0 && r >= 0.05) || (i==size-1 && l >=0.05)) o = 0.0;
        if (l >= 0.05 && o < 0.05 && r < 0.05) ++numOfBorders;
        if (l < 0.05 && o < 0.05 && r >= 0.05) ++numOfBorders;
    }

    return numOfBorders;
}

// calculates lineages (borders / 2) and the trait with the most lineages becomes
// the number of lineages for that generation
int countLineagesForGen(const int t,
                        const std::vector<std::vector<double>> &histX,
                        const std::vector<std::vector<double>> &histP,
                        const std::vector<std::vector<double>> &histQ)
{
    if (t < 0) throw std::invalid_argument("Time can't be negative");
    if (histX.empty()) throw std::invalid_argument("HistX is empty");
    if (histP.empty()) throw std::invalid_argument("HistP is empty");
    if (histQ.empty()) throw std::invalid_argument("HistQ is empty");

    int xBorders = countBorders(histX[t]);
    int pBorders = countBorders(histP[t]);
    int maxBorders = countBorders(histQ[t]);
    if (xBorders > maxBorders) maxBorders = xBorders;
    if (countBorders(histQ[t]) > maxBorders) maxBorders = pBorders;
    return maxBorders / 2;
}

//output all number of lineages for all the generations
void outputLTT(const std::vector<std::vector<double>> &histX,
               const std::vector<std::vector<double>> &histP,
               const std::vector<std::vector<double>> &histQ,
               const kewe_parameters& parameters )
{

    if (histX.empty()) throw std::invalid_argument("HistX is empty");
    if (histP.empty()) throw std::invalid_argument("HistP is empty");
    if (histQ.empty()) throw std::invalid_argument("HistQ is empty");

    std::ofstream LTT("ltt.csv");

    for (int i = 0; i < static_cast<int>(histX.size()); ++i)
        LTT << i * parameters.output_parameters.outputfreq << ","
            << countLineagesForGen(i, histX, histP, histQ) << '\n';
}

void recreate_golden_output(const std::string& filename)
{
  QFile f(":/kewe/kewe_defaultresults");
  assert(f.size());
  f.copy(filename.c_str());
  //assert(is_regular_file(filename));
}
