#ifndef ELLY_RATES_H
#define ELLY_RATES_H

namespace elly {

class rates
{
public:
    rates();

    //all rates are in species per million years
    //mext: per species cladogesis rate on mainland
    double mclad;

    //mext: per species mainland extinction rate on mainland
    double mext;

    //mimm: per species mainland immigration rate on mainland
    double mimm;

    //iext: per species island extinction rate on island
    double iext;

    //iclad: per species island cladogenesis rate on island
    double iclad;

    //iimm: per species island immigration rate on island
    double iimm;

    //bextm: per species mainland extinction rate of species on both mainland and island
    double bextm;

    //bexti: per species island extinction rate of species on both mainland and island
    double bexti;

    //bana: per species anagenesis rate
    double bana;

    //bcladi: per species island cladogenesis rate of species on both mainland and island
    double bcladi;

    //bcladm: per species mainland cladogenesis rate of species on both mainland and island
    double bcladm;
};

} //~namespace elly

#endif // ELLY_RATES_H
