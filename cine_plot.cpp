#include "cine_plot.h"



//define public functions for class plot
void plot::let_grass_grow()
{
    if (m_Grass < 1)
        m_Grass += 0.2;

    if (m_Grass >= 1)
        m_Grass = 1;
}

void plot::grass_consumption()
{
    m_Grass = 0;
}

void plot::setRisk(double risk)
{
    m_Risk = risk;
}

/*
void plot::setPosition(int x, int y)
{
    m_Xcoor = x;
    m_Ycoor = y;
}
*/
