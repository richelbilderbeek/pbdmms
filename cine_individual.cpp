#include "cine_individual.h"

//define public functions
void individual::setPosition(int x, int y){
    m_Xcoor = x;
    m_Ycoor = y;
}

void individual::update_food(double foodintake) {
    m_food += foodintake;
}

//void individual::set_fitness(double fitness_new) {
//    m_fitness = fitness_new;
//}
