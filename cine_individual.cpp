#include "cine_individual.h"

//define public functions


void individual::setPosition(int x, int y){
    m_Xcoor = x;
    m_Ycoor = y;
}

void individual::food_uptake(double foodintake) {
    m_food += foodintake;
}

void individual::set_weight(int i, float new_weight){
    m_weights[i] = new_weight;
}

