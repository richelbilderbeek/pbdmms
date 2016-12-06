#include "elly_rates.h"
#include <vector>
#include <numeric>

elly::rates::rates()
  : mclad{0.0}, mext{0.0}, mimm{0.0} , iext{0.0} , iclad{0.0} , iimm{0.0} , bextm{0.0},
    bexti{0.0}, bana{0.0}, bcladi{0.0} , bcladm{0.0}
{


}

std::vector<double> elly::to_ratesvector(const rates& r) noexcept
{
  return {
    r.mclad,
    r.mext,
    r.mimm,
    r.iext,
    r.iclad,
    r.iimm,
    r.bextm,
    r.bexti,
    r.bana,
    r.bcladi,
    r.bcladm
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
