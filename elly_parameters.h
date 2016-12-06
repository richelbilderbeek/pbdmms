#ifndef ELLY_PARAMETERS_H
#define ELLY_PARAMETERS_H
#include <cmath>
#include <stdexcept>
#include <iosfwd>
#include <utility>

namespace elly {

class parameters
{
public:
    parameters(
    //int mainland_species,
    //int island_species,
    //int both_species,
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

    //int get_mainland_species() const noexcept {return mainland_species; }
    //int get_island_species() const noexcept { return island_species; }
    //int get_both_species() const noexcept { return both_species; }
    double get_mig_rate_is() const noexcept{ return m_rate_mig_is;}
    double get_mig_rate_main() const noexcept {return m_rate_mig_main;}
    double get_ext_rate_is() const noexcept { return m_rate_ext_is;}
    double get_ext_rate_main() const noexcept { return m_rate_ext_main;}
    double get_clado_rate_is() const noexcept { return m_rate_clado_is;}
    double get_clado_rate_main() const noexcept { return m_rate_clado_main;}
    double get_ana_rate() const noexcept { return m_rate_ana;}

    parameters create_parameters() noexcept;

private:
     double m_rate_clado_is;
     double m_rate_clado_main;
     double m_rate_ana;
     double m_rate_ext_is;
     double m_rate_ext_main;
     double m_rate_mig_is;
     double m_rate_mig_main;
     int m_carryingcap_is;
     int m_carryingcap_main;
     unsigned int m_rng_seed;
};



}//~namespace elly
#endif // ELLY_PARAMETERS_H
