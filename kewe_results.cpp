#include <fstream>
#include <cmath>
#include <cassert>
#include <iostream>
#include <QFile>
#include <algorithm>
#include <string>
#include <random>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include "count_undirected_graph_connected_components.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "count_max_number_of_pieces.h"

#include "kewe_attractivenesses.h"
#include "kewe_genotype_graph.h"
#include "kewe_results.h"
#include "kewe_parameters.h"
#include "kewe_ses.h"

kewe::genotypes kewe::calc_average_genotype(const individuals& pop)
{
  genotypes averages;

  for(auto i=std::begin(pop);i!=std::end(pop);i++)
    {

      averages.m_x+=i->get_eco_trait();
      averages.m_p+=i->get_fem_pref();
      averages.m_q+=i->get_male_trait();

    }
    averages.m_x/=static_cast<double>(pop.size());
    averages.m_p/=static_cast<double>(pop.size());
    averages.m_q/=static_cast<double>(pop.size());

    return averages;
}

int kewe::calc_j_trait(const int histw, const double trait, const parameters& parameters)
{
  int j_trait = static_cast<int>(histw/2.0+trait/parameters.m_output_parameters.histbinx);
  if(j_trait>=histw) j_trait=histw-1;
  if(j_trait<0) j_trait=0;
  return j_trait;
}

void kewe::calculate_rho(
    const individuals& pop,
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
    double dxi=i->get_eco_trait()-averageGenotypes.m_x;
    double dpi=i->get_fem_pref()-averageGenotypes.m_p;
    double dqi=i->get_male_trait()-averageGenotypes.m_q;

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

void kewe::calculate_s(
    const individuals& pop,
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
    double dxi=i->get_eco_trait()-averageGenotypes.m_x;
    double dpi=i->get_fem_pref()-averageGenotypes.m_p;
    double dqi=i->get_male_trait()-averageGenotypes.m_q;

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

///Thank you jobo
int kewe::count_good_species(
    const individuals& pop,
    const simulation_parameters& parameters
    )
{
  if (pop.empty()) return 0;
  if (static_cast<int>(pop.size()) == 1) return 1;

  const attractivenesses as{calc_attractivenesses(pop, parameters)};

  genotype_graph g;
  add_vertices(pop, g);
  add_edges(as, g, parameters);

/*
  { //Don't run in travis!!!
    // Create picture of all genotypes and their connections
    const std::string dot_filename{"kewe_count_good_species.dot"};
    const std::string svg_filename{"kewe_count_good_species.svg"};
    const std::string png_filename{"kewe_count_good_species.png"};
    std::ofstream f(dot_filename);
    boost::write_graphviz(f, g,
      [g](std::ostream& os, const auto iter)
      {
        os << "[label=\"" << g[iter] << "\"]";
      }
    );
    f.close();
    convert_dot_to_svg(dot_filename, svg_filename);
    convert_svg_to_png(svg_filename, png_filename);
    std::system("display kewe_count_good_species.png");
  }
  */
  return count_undirected_graph_connected_components(g);
}


void kewe::output_data(
    std::ofstream& out,
    const int t,
    const genotypes& averageGenotypes,
    const result_variables& result,
    const parameters& parameters
    )
{
  out<<t<<","<<static_cast<double>(parameters.m_sim_parameters.popsize)<<","
     <<result.m_rhoxp.back()<<","<<result.m_rhoxq.back()<<","<<result.m_rhopq.back()<<","
     <<result.m_sx.back()<<","<<result.m_sq.back()<<","<<result.m_sp.back();

  if (!parameters.m_output_parameters.is_silent)
  {
    std::clog
      << t << " "
      << static_cast<double>(parameters.m_sim_parameters.popsize) << " "
      << result.m_rhoxp.back()<<" "<<result.m_rhoxq.back()<<" "<<result.m_rhopq.back()
      << '\n'
      << averageGenotypes.m_x<<" "<<averageGenotypes.m_p<<" "<<averageGenotypes.m_q<<" "
      << result.m_sx.back()<<" "<<result.m_sq.back()<<" "<<result.m_sp.back()<< '\n';
  }
}

void kewe::output_histogram(std::ofstream& out,
                 const std::vector<double>& hist,
                 std::vector<std::vector<double>>& hist_all_gens,
                 const int histw
                 )
{

  /// normalize output
  double max=0.0;

  max = *std::max_element(hist.begin(), hist.end());

  // temporary histograms for next iteration
  std::vector<double> histGen;
  histGen.reserve(static_cast<size_t>(histw));
  for(int j=0;j<histw;j++)
  {
      assert(j >= 0);
      assert(j < static_cast<int>(hist.size()));
      out<<","<<hist[j]/max;
      histGen.push_back(hist[j]/max);
  }
  hist_all_gens.push_back(histGen);

}

void kewe::output_histograms(
    std::ofstream& out,
    const parameters& parameters,
    const individuals& pop,
    std::vector<std::vector<double>> &histX,
    std::vector<std::vector<double>> &histP,
    std::vector<std::vector<double>> &histQ
    )
{

  const int histw = parameters.m_output_parameters.histw;

  assert(histw >= 0);
  std::vector<double> histx(histw, 0.0);
  assert(histw == static_cast<int>(histx.size()));
  std::vector<double> histp(histw, 0.0);
  std::vector<double> histq(histw, 0.0);

  const double delta=1.0/static_cast<double>(parameters.m_sim_parameters.popsize);

  for(auto i=std::begin(pop);i!=std::end(pop);i++)
  {
    int jx = calc_j_trait(histw, i->get_eco_trait(), parameters);
    int jp = calc_j_trait(histw, i->get_fem_pref(), parameters);
    int jq = calc_j_trait(histw, i->get_male_trait(), parameters);

    assert(jx >= 0);
    assert(jx < static_cast<int>(histx.size()));
    assert(jp >= 0);
    assert(jp < static_cast<int>(histp.size()));
    assert(jq >= 0);
    assert(jq < static_cast<int>(histq.size()));
    histx[jx]+=delta;
    histp[jp]+=delta;
    histq[jq]+=delta;
   }

  output_histogram(out, histx, histX, histw);
  output_histogram(out, histp, histP, histw);
  output_histogram(out, histq, histQ,  histw);

  out << '\n';
}

void kewe::output(
      const int t,
      std::vector<std::vector<double>> &histX,
      std::vector<std::vector<double>> &histP,
      std::vector<std::vector<double>> &histQ,
      const parameters& parameters,
      const individuals& pop,
      result_variables& result,
      std::vector<std::pair<int,int>>& ltt_plot
      )
{
  result.m_t.push_back(t);
  result.m_popsize.push_back(static_cast<double>(pop.size()));


  std::ofstream out(parameters.m_output_parameters.outputfilename);

  genotypes averageGenotypes = calc_average_genotype(pop);

  calculate_rho(pop, averageGenotypes, result);
  calculate_s(pop, averageGenotypes, result);
  output_data(out, t, averageGenotypes, result, parameters);
  output_histograms(out, parameters, pop, histX, histP, histQ);
  output_ltt(pop, t, parameters, ltt_plot);

}

void kewe::output_ltt(
    const individuals& pop,
    const int t,
    const parameters& p,
    std::vector<std::pair<int,int>>& ltt_plot
    )
{
  const std::pair<int,int> output_pair(
    t, count_good_species(pop,p.m_sim_parameters)
  );
  ltt_plot.push_back(output_pair);
}

///Old code for "hack"
/*void count_num_border(
    const double l,
    const double o,
    const double r,
    int& numOfBorders)
{
  if (l >= 0.05 && o < 0.05 && r < 0.05) ++numOfBorders;
  else if (l < 0.05 && o < 0.05 && r >= 0.05) ++numOfBorders;
}



// Count number of borders (from 0 to >0 or from >0 to 0) in a histogram
int countBorders(const std::vector<double> &histogram)
{
    if (histogram.empty()) throw std::invalid_argument("Histogram is empty");

    int size = static_cast<int>(histogram.size());
    int numOfBorders{0};
    for (int i = 0; i<size; ++i)
    {
        assert(i >= 0);
        assert(i < static_cast<int>(histogram.size()));
        double l, r, o = histogram[i];
        if (i==0) l = 0.0;
        else l = histogram[i-1];
        if (i==size-1) r = 0.0;
        else r = histogram[i+1];

        bool at_left_border = i==0 && r>0.05;
        bool at_right_border = i==size-1 && l >= 0.05;

        if (at_left_border || at_right_border) o = 0.0;
        count_num_border(l, o, r, numOfBorders);
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

  throw_count_lineages(t, histX, histP, histQ);

  assert(t < static_cast<int>(histX.size()));
  int xBorders = countBorders(histX[t]);

  assert(t < static_cast<int>(histP.size()));
  int pBorders = countBorders(histP[t]);

  assert(t < static_cast<int>(histQ.size()));
  int maxBorders = countBorders(histQ[t]);

  if (xBorders > maxBorders) maxBorders = xBorders;
  if (pBorders > maxBorders) maxBorders = pBorders;
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
        LTT << i * parameters.m_output_parameters.outputfreq << ","
            << countLineagesForGen(i, histX, histP, histQ) << '\n';
}

void throw_count_lineages(const int t,
                          const std::vector<std::vector<double>>& histX,
                          const std::vector<std::vector<double>>& histP,
                          const std::vector<std::vector<double>>& histQ
                          )
{
  if (t < 0) throw std::invalid_argument("Time can't be negative");
  else if (histX.empty()) throw std::invalid_argument("HistX is empty");
  else if (histP.empty()) throw std::invalid_argument("HistP is empty");
  else if (histQ.empty()) throw std::invalid_argument("HistQ is empty");
}



void recreate_golden_output(const std::string& filename)
{
  QFile f(":/kewe/kewe_defaultresults");
  assert(f.size());
  f.copy(filename.c_str());
  //assert(is_regular_file(filename));
}*/
