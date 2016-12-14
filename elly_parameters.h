#ifndef ELLY_PARAMETERS_H
#define ELLY_PARAMETERS_H
#include <cmath>
#include <stdexcept>
#include <iosfwd>
#include <iostream>
#include <utility>

namespace elly {

class parameters
{
public:
    parameters(
    const double rate_clado_is,
    const double rate_clado_main,
    const double rate_ana,
    const double rate_ext_is,
    const double rate_ext_main,
    const double rate_mig_is,
    const double rate_mig_main,
    const int carryingcap_is,
    const int carryingcap_main,
    const unsigned int rng_seed
    );

    double get_mig_rate_is() const noexcept{ return m_rate_mig_is;}
    double get_mig_rate_main() const noexcept {return m_rate_mig_main;}
    double get_ext_rate_is() const noexcept { return m_rate_ext_is;}
    double get_ext_rate_main() const noexcept { return m_rate_ext_main;}
    double get_clado_rate_is() const noexcept { return m_rate_clado_is;}
    double get_clado_rate_main() const noexcept { return m_rate_clado_main;}
    double get_ana_rate() const noexcept { return m_rate_ana;}
    double get_carryingcap_is() const noexcept { return m_carryingcap_is;}
    double get_carryingcap_main() const noexcept { return m_carryingcap_main;}
    int get_rng_seed() const noexcept { return m_rng_seed;}

private:
     const double m_rate_clado_is;
     //rate of cladogenesis on island per species

     const double m_rate_clado_main;
     const double m_rate_ana;
     const double m_rate_ext_is;
     const double m_rate_ext_main;
     const double m_rate_mig_is;
     const double m_rate_mig_main;
     const int m_carryingcap_is;
     const int m_carryingcap_main;
     const unsigned int m_rng_seed;
};

///rates extinction and cladogenesis same for mainland and island for now
parameters create_parameters_set1() noexcept;

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;

}//~namespace elly
#endif // ELLY_PARAMETERS_H
