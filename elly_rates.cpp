#include "elly_rates.h"
#include <vector>
#include <numeric>
#include <stdexcept>

elly::rates::rates(
    double mclad,
    double mext,
    double mimm,
    double iext,
    double iclad,
    double iimm,
    double bextm,
    double bana,
    double bcladi,
    double bcladm
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

