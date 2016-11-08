#include <fstream>
#include <cmath>
#include <cassert>
#include <iostream>
#include <QFile>

#include "kewe_results.h"
#include "kewe_parameters.h"

void output(bigint t,
            std::vector<std::vector<double>> &histX,
            std::vector<std::vector<double>> &histP,
            std::vector<std::vector<double>> &histQ,
            const kewe_parameters& parameters,
            const std::vector<indiv>& pop
            )
{
  std::ofstream out(parameters.outputfilename);
  const int histw = parameters.histw;
  double avgp=0.0,avgq=0.0,avgx=0.0, rhoxp,rhoxq,rhopq,
      ssxx=0.0,ssxp=0.0,sspp=0.0,ssxq=0.0,ssqq=0.0,sspq=0.0,dxi,dpi,dqi,delta,
      maxx=0.0,maxp=0.0,maxq=0.0, sx,sp,sq,xi,pi,qi;
  int j,jx,jp,jq;

  delta=1.0/static_cast<double>(pop.size());

  std::vector<double> histx(histw, 0.0);
  std::vector<double> histp(histw, 0.0);
  std::vector<double> histq(histw, 0.0);

  for(auto i=std::begin(pop);i!=std::end(pop);i++)
  {
      avgx+=i->_x();
      avgp+=i->_p();
      avgq+=i->_q();

  }
  avgx/=static_cast<double>(pop.size());
  avgp/=static_cast<double>(pop.size());
  avgq/=static_cast<double>(pop.size());

  for(auto i=std::begin(pop);i!=std::end(pop);i++)
  {
      xi=i->_x();
      pi=i->_p();
      qi=i->_q();

      dxi=xi-avgx;
      dpi=pi-avgp;
      dqi=qi-avgq;
      ssxx+=dxi*dxi;
      ssxp+=dxi*dpi;
      ssxq+=dxi*dqi;
      sspp+=dpi*dpi;
      sspq+=dpi*dqi;
      ssqq+=dqi*dqi;

      jx=int(histw/2.0+xi/parameters.histbinx);
      jp=int(histw/2.0+pi/parameters.histbinp);
      jq=int(histw/2.0+qi/parameters.histbinq);

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
    rhoxp=ssxp/sqrt(ssxx*sspp);
    rhoxq=ssxq/sqrt(ssxx*ssqq);
    rhopq=sspq/sqrt(sspp*ssqq);
    sx=sqrt(ssxx/(static_cast<double>(pop.size())-1.0));
    sp=sqrt(sspp/(static_cast<double>(pop.size())-1.0));
    sq=sqrt(ssqq/(static_cast<double>(pop.size())-1.0));
    out<<t<<","<<static_cast<double>(pop.size())<<","<<rhoxp<<","<<rhoxq<<","<<rhopq<<","<<sx<<","<<sp<<","<<sq;
    std::cout<<t<<" "<<static_cast<double>(pop.size())<<" "<<rhoxp<<" "<<rhoxq<<" "<<rhopq<< std::endl
        <<avgx<<" "<<avgp<<" "<<avgq<<" "<<sx<<" "<<sp<<" "<<sq<<std::endl;

    std::vector<double> histXGen;
    std::vector<double> histPGen;
    std::vector<double> histQGen;


    assert(histXGen.empty());
    assert(histPGen.empty());
    assert(histQGen.empty());

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
    return;
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
        LTT << i * parameters.outputfreq << "," << countLineagesForGen(i, histX, histP, histQ) << '\n';
}

void recreate_golden_output(const std::string& filename)
{
  QFile f(":/kewe/kewe_defaultresults");
  assert(f.size());
  f.copy(filename.c_str());
  //assert(is_regular_file(filename));
}
