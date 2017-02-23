#include "cine_plot.h"
#include <algorithm>    // for shuffle function


//define public functions for class plot
void plot::let_grass_grow()
{
    if (m_Grass < 1)
        m_Grass += 0.2;

    if (m_Grass >= 1)
        m_Grass = 1;
    //Doesn't work, to 1 straight away!
    //m_Grass = std::max(1.0, m_Grass + 0.2);
}

void plot::grass_consumption()
{
    m_Grass = 0;
}

void plot::setRisk(double risk)
{
    m_Risk = risk;
}

void plot::set_predclues(double predclues)
{
    m_predclues = predclues;
}

void plot::set_preyclues(double preyclues)
{
    m_preyclues = preyclues;
}
