#ifndef CINE_INDIVIDUAL_H
#define CINE_INDIVIDUAL_H

#include <vector>

//initialize class individual
class individual
{

public:
    void setPosition(int x, int y);
    int xposition() const { return m_Xcoor; }
    int yposition() const { return m_Ycoor; }
    void food_uptake(double foodintake);
    double return_food() const { return m_food; }
    double return_weight(int i) {return m_weights[i];}
    std::vector<double> return_weightvct(){return m_weights;}
    int return_weightlength() const {return m_weights.size();}
    void set_weight(int i, double new_weight);

    individual()
      : m_Xcoor{0}, m_Ycoor{0}, m_food{0.0}, m_weights{16, 0.5}
    {
        //All done!
    }


private:
    int m_Xcoor;
    int m_Ycoor;
    double m_food;
    std::vector<double> m_weights;
};

#endif // CINE_INDIVIDUAL_H
