#include "elly_event_rates.h"
#include <vector>
#include <numeric>
#include <stdexcept>
#include <numeric>

#include "elly_simulation.h"

elly::event_rates::event_rates( //!OCLINT It would only make the code more complex if parameters are seperated by location
    const double mclad,
    const double mext,
    const double mimm,
    const double iext,
    const double iclad,
    const double iimm,
    const double bextm,
    const double bexti,
    const double bana,
    const double bcladi,
    const double bcladm ) :
    m_mclad{mclad},
    m_mext{mext},
    m_mimm{mimm},
    m_iext{iext},
    m_iclad{iclad},
    m_iimm{iimm},
    m_bextm{bextm},
    m_bexti{bexti},
    m_bana{bana},
    m_bcladi{bcladi},
    m_bcladm{bcladm},
    m_dd_rates_bcladi(1000, 0.0),
    m_dd_rates_mimm(1000, 0.0),
    m_dd_rates_iclad(1000,0.0)
{


}


std::vector<double> elly::to_ratesvector(const event_rates& r) noexcept
{
  return {
    r.get_mclad(),
    r.get_mext(),
    r.get_mimm(),
    r.get_iext(),
    r.get_iclad(),
    r.get_iimm(),
    r.get_bextm(),
    r.get_bexti(),
    r.get_bana(),
    r.get_bcladi(),
    r.get_bcladm()
  };
}

double elly::calc_sumrates(const event_rates& r) noexcept
{
  const auto rates = to_ratesvector(r);
  return std::accumulate(
    std::begin(rates),
    std::end(rates),
    0.0
  );
}

elly::event_rates elly::calculate_rates(
  const parameters& p,
  const simulation& s
)
{
  event_rates r;

  //Number of specie
  const int n_main_only{static_cast<int>(s.get_species_mainland().size())};
  const int n_island_only{static_cast<int>(s.get_species_island().size())};
  const int n_both{static_cast<int>(s.get_species_both().size())};
  const int n_main{n_main_only + n_both};
  const int n_island{n_island_only + n_both};
  r.set_mext( p.get_ext_rate_main() * n_main_only);
  //r.set_mimm( p.get_mig_rate_main() * nm * (1 - ni / p.get_carryingcap_is()))
  r.set_iext( p.get_ext_rate_is() * n_island_only);
  r.set_iimm( p.get_mig_rate_is() * n_island * (1 - n_main / p.get_carryingcap_main() ));
  r.set_bextm( p.get_ext_rate_main() * n_both);
  r.set_bexti( p.get_ext_rate_is() * n_both);
  r.set_bana( p.get_ana_rate() * n_both);

  //if statements to avoid dividing by 0
  if(n_main == 0){
      r.set_mclad(0.0);
      r.set_bcladm(0.0);
    } else{
      r.set_mclad(p.get_clado_rate_main() * (n_main_only / n_main) * (1 - n_main / p.get_carryingcap_main()));
      r.set_bcladm( p.get_clado_rate_main() * (n_both / n_main ) * ( 1 - n_main / p.get_carryingcap_main()));
    }
  std::vector<double> temp_bcladi = r.get_dd_rates_bcladi();
  std::vector<double> temp_mimm = r.get_dd_rates_mimm();
  std::vector<double> temp_iclad = r.get_dd_rates_iclad();

  calculate_rates_per_clade(species_in_clades, p, r, n_island_only, n_both, n_main_only);
  r.set_bcladi(std::accumulate(temp_bcladi.begin(), temp_bcladi.end() , 0 ));
  r.set_mimm(std::accumulate(temp_mimm.begin(), temp_mimm.end(), 0 ));
  r.set_iclad(std::accumulate(temp_iclad.begin(), temp_iclad.end(), 0 ));
  //setting diversity dependent rates from their own rates vectors


 return r;
}

void elly::calculate_rates_per_clade(
  const std::vector<int>& species_in_clades,
  const parameters& p,
  event_rates& r,
  const int io,
  const int bo,
  const int mo
)
{
  const int n_clades{static_cast<int>(species_in_clades.size())};
  std::vector<double> temp_bcladi(n_clades, 0);
  std::vector<double> temp_mimm(n_clades, 0);
  std::vector<double> temp_iclad(n_clades, 0);

  //for every clade, calculating the immigration from mainland rate
  //and rate of island cladogenesis
  for(int i{0}; i != n_clades; ++i)
  {
    const int k_i{p.get_carryingcap_is()}; //Carrying capacity island
    const int n_species{species_in_clades[i]}; //Number of species in this clade
    const double k_i_d{static_cast<double>(k_i)}; //as double
    const double n_species_d{static_cast<double>(n_species)}; //as double
    const double mo_d{static_cast<double>(mo)};
    const double io_d{static_cast<double>(io)};
    const double bo_d{static_cast<double>(bo)};

    temp_mimm[i]   = p.get_mig_rate_main() * mo_d * (1.0 - (n_species_d / k_i_d)); //why not p.get_carryingcap_main?
    temp_iclad[i]  = p.get_clado_rate_is() * io_d * (1.0 - (n_species_d / k_i_d));
    temp_bcladi[i] = p.get_clado_rate_is() * bo_d * (1.0 - (n_species_d / k_i_d));
  }

r.set_dd_rates_bcladi(temp_bcladi);
r.set_dd_rates_iclad(temp_iclad);
r.set_dd_rates_mimm(temp_mimm);

}


void elly::event_rates::set_mclad(const double mclad)
{
  if (mclad < 0.0)
    throw std::invalid_argument("mclad must be positive");
  m_mclad = mclad;
}
void elly::event_rates::set_mext(const double mext)
{
  if(mext < 0.0)
    throw std::invalid_argument("mext must be positive");
  m_mext = mext;
}
void elly::event_rates::set_mimm(const double mimm)
{
  if(mimm < 0.0)
    throw std::invalid_argument("mimm must be positive");
  m_mimm = mimm;
}

void elly::event_rates::set_iext(const double iext)
{
  if(iext < 0.0)
    throw std::invalid_argument("iext must be positive");
  m_iext = iext;
}

void elly::event_rates::set_iclad(const double iclad)
{
  if(iclad < 0.0)
    throw std::invalid_argument("iclad must be positive");
  m_iclad = iclad;
}
void elly::event_rates::set_iimm(const double iimm)
{
  if(iimm < 0.0)
    throw std::invalid_argument("iimm must be positive");
  m_iimm = iimm;
}
void elly::event_rates::set_bextm(const double bextm)
{
  if(bextm < 0.0)
    throw std::invalid_argument("bextm must be positive");
  m_bextm = bextm;
}
void elly::event_rates::set_bexti(const double bexti)
{
  if(bexti < 0.0)
    throw std::invalid_argument("bexti must be positive");
  m_bexti = bexti;
}
void elly::event_rates::set_bana(const double bana)
{
  if(bana < 0.0)
    throw std::invalid_argument("bana must be positive");
  m_bana = bana;
}
void elly::event_rates::set_bcladi(const double bcladi)
{
  if(bcladi < 0.0)
    throw std::invalid_argument("bcladi must be positive");
  m_bcladi = bcladi;
}
void elly::event_rates::set_bcladm(const double bcladm)
{
  if(bcladm < 0.0)
    throw std::invalid_argument("bcladm must be positive");
  m_bcladm = bcladm;
}

void elly::event_rates::set_dd_rates_bcladi(
  const std::vector<double>& dd_rates_bcladi
)
{
  for (const double i: dd_rates_bcladi)
  {
    if(i < 0.0)
    {
      throw std::invalid_argument("dd rates bcladi must all be positive");
    }
  }
  m_dd_rates_bcladi = dd_rates_bcladi;
}

void elly::event_rates::set_dd_rates_mimm(const std::vector<double>& dd_rates_mimm)
{
  for (const double i: dd_rates_mimm)
  {
    if(i < 0.0)
    {
      throw std::invalid_argument("dd rates mimm must all be positive");
    }
  }

  m_dd_rates_mimm = dd_rates_mimm;
}

void elly::event_rates::set_dd_rates_iclad(const std::vector<double>& dd_rates_iclad)
{
  for (const double i: dd_rates_iclad)
  {
    if(i < 0.0)
    {
      throw std::invalid_argument("dd rates iclad must all be positive");
    }
  }

  m_dd_rates_iclad = dd_rates_iclad;
}
