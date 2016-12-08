#include "elly_rates.h"
#include <vector>
#include <numeric>
#include <stdexcept>

elly::rates::rates(
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
    const double bcladm
)
  : m_mclad{mclad},
    m_mext{mext},
    m_mimm{mimm},
    m_iext{iext},
    m_iclad{iclad},
    m_iimm{iimm},
    m_bextm{bextm},
    m_bexti{bexti},
    m_bana{bana},
    m_bcladi{bcladi},
    m_bcladm{bcladm}
{
  if (m_mclad < 0.0)
    throw std::invalid_argument("mclad must be positive");
  if (m_mext < 0.0)
    throw std::invalid_argument("mext must be positive");
  if (m_mimm < 0.0)
    throw std::invalid_argument("mimm must be positive");
  if (m_iext < 0.0)
    throw std::invalid_argument("iext must be positive");
  if (m_iclad < 0.0)
    throw std::invalid_argument("iclad must be positive");
  if (m_iimm < 0.0)
    throw std::invalid_argument("iimm must be positive");
  if (m_bextm < 0.0)
    throw std::invalid_argument("bextm must be positive");
  if (m_bexti < 0.0)
    throw std::invalid_argument("bexti must be positive");
  if (m_bana < 0.0)
    throw std::invalid_argument("bana must be positive");
  if (m_bcladi < 0.0)
    throw std::invalid_argument("bcladi must be positive");
  if (m_bcladm < 0.0)
    throw std::invalid_argument("bcladm must be positive");
}

std::vector<double> elly::to_ratesvector(const rates& r) noexcept
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

double elly::calc_sumrates(const rates& r) noexcept
{
  const auto rates = to_ratesvector(r);
  return std::accumulate(
    std::begin(rates),
    std::end(rates),
    0.0
  );
}

elly::rates elly::calculate_rates(const parameters& p, int mo , int io , int bo)
{
  elly::rates r;
  mo = static_cast<double>(mo);
  io = static_cast<double>(io);
  bo = static_cast<double>(bo);

  int nm   = mo + bo;
  int ni   = io + bo;
  r.set_mext( p.get_ext_rate_main() * mo);
  r.set_mimm( p.get_mig_rate_main() * nm * (1 - ni / p.get_carryingcap_is()));
  r.set_iext( p.get_ext_rate_is() * io);
  r.set_iimm( p.get_mig_rate_is() * ni * (1 - nm / p.get_carryingcap_main() ));
  r.set_bextm( p.get_ext_rate_main() * bo);
  r.set_bexti( p.get_ext_rate_is() * bo);
  r.set_bana( p.get_ana_rate() * bo);

  //if statements to avoid dividing by 0
  if(nm == 0){
      r.set_mclad(0.0);
      r.set_bcladm(0.0);
    } else{
      r.set_mclad(p.get_clado_rate_main() * (mo / nm) * (1 - nm / p.get_carryingcap_main()));
      r.set_bcladm( p.get_clado_rate_main() * (bo / nm ) * ( 1 - nm / p.get_carryingcap_main()));
    }

  if(ni == 0){
      r.set_iclad( 0);
      r.set_bcladi( 0);
    } else{
      r.set_iclad( p.get_clado_rate_is() * (io / ni) * (1 - ni / p.get_carryingcap_is()));
      r.set_bcladi( p.get_clado_rate_is() * (bo / ni) * ( 1 - ni / p.get_carryingcap_is()));
    }
 return r;
}

void elly::rates::set_mclad(const double mclad)
{
  if (mclad < 0.0)
    throw std::invalid_argument("mclad must be positive");
  m_mclad = mclad;
}
void elly::rates::set_mext(const double mext)
{
  if(mext < 0.0)
    throw std::invalid_argument("mext must be positive");
  m_mext = mext;
}
void elly::rates::set_mimm(const double mimm)
{
  if(mimm < 0.0)
    throw std::invalid_argument("mimm must be positive");
  m_mimm = mimm;
}

void elly::rates::set_iext(const double iext)
{
  if(iext < 0.0)
    throw std::invalid_argument("iext must be positive");
  m_iext = iext;
}

void elly::rates::set_iclad(const double iclad)
{
  if(iclad < 0.0)
    throw std::invalid_argument("iclad must be positive");
  m_iclad = iclad;
}
void elly::rates::set_iimm(const double iimm)
{
  if(iimm < 0.0)
    throw std::invalid_argument("iimm must be positive");
  m_iimm = iimm;
}
void elly::rates::set_bextm(const double bextm)
{
  if(bextm < 0.0)
    throw std::invalid_argument("bextm must be positive");
  m_bextm = bextm;
}
void elly::rates::set_bexti(const double bexti)
{
  if(bexti < 0.0)
    throw std::invalid_argument("bexti must be positive");
  m_bexti = bexti;
}
void elly::rates::set_bana(const double bana)
{
  if(bana < 0.0)
    throw std::invalid_argument("bana must be positive");
  m_bana = bana;
}
void elly::rates::set_bcladi(const double bcladi)
{
  if(bcladi < 0.0)
    throw std::invalid_argument("bcladi must be positive");
  m_bcladi = bcladi;
}
void elly::rates::set_bcladm(const double bcladm)
{
  if(bcladm < 0.0)
    throw std::invalid_argument("bcladm must be positive");
  m_bcladm = bcladm;
}

