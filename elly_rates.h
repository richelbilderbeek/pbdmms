#ifndef ELLY_RATES_H
#define ELLY_RATES_H
#include <vector>

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



private:
    //all rates are in species per million years
    //mext: per species cladogesis rate on mainland
    double m_mclad;

    //mext: per species mainland extinction rate on mainland
    double m_mext;

    //mimm: per species mainland immigration rate on mainland
    double m_mimm;

    //iext: per species island extinction rate on island
    double m_iext;

    //iclad: per species island cladogenesis rate on island
    double m_iclad;

    //iimm: per species island immigration rate on island
    double m_iimm;

    //bextm: per species mainland extinction rate of species on both mainland and island
    double m_bextm;

    //bexti: per species island extinction rate of species on both mainland and island
    double m_bexti;

    //bana: per species anagenesis rate
    double m_bana;

    //bcladi: per species island cladogenesis rate of species on both mainland and island
    double m_bcladi;

    //bcladm: per species mainland cladogenesis rate of species on both mainland and island
    double m_bcladm;

};

//ratesvector: all rates stored in a vector
std::vector<double> to_ratesvector(const rates& r) noexcept;

//sumrates: sum of all rates
double calc_sumrates(const rates& r) noexcept;

} //~namespace elly

#endif // ELLY_RATES_H
