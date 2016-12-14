#ifndef ELLY_RATES_H
#define ELLY_RATES_H
#include <vector>
#include "elly_parameters.h"

namespace elly {

class rates
{
public:
    rates(
      const double mclad = 0.0,
      const double mext = 0.0,
      const double mimm = 0.0,
      const double iext = 0.0,
      const double iclad = 0.0,
      const double iimm = 0.0,
      const double bextm = 0.0,
      const double bexti = 0.0,
      const double bana = 0.0,
      const double bcladi = 0.0,
      const double bcladm = 0.0
   );

   double get_mclad() const noexcept { return m_mclad; }
   double get_mext() const noexcept { return m_mext;  }
   double get_mimm() const noexcept {  return m_mimm;  }
   double get_iext() const noexcept {  return m_iext;  }
   double get_iclad() const noexcept {  return m_iclad;  }
   double get_iimm() const noexcept {  return m_iimm;  }
   double get_bextm() const noexcept {  return m_bextm;  }
   double get_bexti() const noexcept {  return m_bexti;  }
   double get_bana() const noexcept {  return m_bana;  }
   double get_bcladi() const noexcept {  return m_bcladi;  }
   double get_bcladm() const noexcept {  return m_bcladm;  }
   std::vector<double> get_dd_rates_bcladi() const noexcept { return m_dd_rates_bcladi;}
   std::vector<double> get_dd_rates_mimm() const noexcept { return m_dd_rates_mimm;}
   std::vector<double> get_dd_rates_iclad() const noexcept { return m_dd_rates_iclad;}
   void set_mclad(const double mclad);
   void set_mext(const double mext);
   void set_mimm(const double mimm);
   void set_iext(const double iext);
   void set_iclad(const double iclad);
   void set_iimm(const double iimm);
   void set_bextm(const double bextm);
   void set_bexti(const double bexti);
   void set_bana(const double bana);
   void set_bcladi(const double bcladi);
   void set_bcladm(const double bcladm);
   void set_dd_rates_bcladi(std::vector<double> dd_rates_bcladi);
   void set_dd_rates_mimm(std::vector<double> dd_rates_mimm);
   void set_dd_rates_iclad(std::vector<double> dd_rates_iclad);



private:
    //all rates are in species per million years
    //mext: cladogesis rate of all species on mainland
    double m_mclad;

    //mext: mainland extinction rate of all species on mainland
    double m_mext;

    //mimm: mainland immigration rate of all species on mainland
    double m_mimm;

    //iext: island extinction rate of all species on island
    double m_iext;

    //iclad: island cladogenesis rate of all species on island
    double m_iclad;

    //iimm: island immigration rate of all speceis on island
    double m_iimm;

    //bextm: mainland extinction rate of species on both mainland and island
    double m_bextm;

    //bexti: per species island extinction rate of species on both mainland and island
    double m_bexti;

    //bana: anagenesis rate of all species
    double m_bana;

    //bcladi: island cladogenesis rate of species on both mainland and island
    double m_bcladi;

    //bcladm: mainland cladogenesis rate of species on both mainland and island
    double m_bcladm;

    //per clade rates of both habitats cladogenesis on island
    std::vector<double> m_dd_rates_bcladi;

    //per clade rates of mainland immigration
    std::vector<double> m_dd_rates_mimm;

    //per clade rates of island cladogenesis
    std::vector<double> m_dd_rates_iclad;
};

//Calculates the rates
//mo number of mainland-only species
rates calculate_rates(const parameters& p, int mo , int io , int bo,
                      std::vector<int> species_in_clades);

//calculate rates per clade, dependant on diversity on island.
void calculate_rates_per_clade(std::vector<int> species_in_clades,
                               const parameters& p, rates& r,
                               int io, int bo, int mo);

//ratesvector: all rates stored in a vector
std::vector<double> to_ratesvector(const rates& r) noexcept;

//sumrates: sum of all rates
double calc_sumrates(const rates& r) noexcept;

} //~namespace elly

#endif // ELLY_RATES_H
