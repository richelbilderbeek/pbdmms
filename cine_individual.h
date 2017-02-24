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
    void food_update(double foodintake);
    double return_food() const { return m_food; }
    const std::vector<double>& weights() const {return m_weights;}
    void set_weight(int i, double new_weight);
    char type() const {return m_type;}
    void type(char type);
    char smart() const {return m_smart;}
    void smart(char smart);
    char evolve() const {return m_evolve;}
    void evolve(char evolve);

    individual()
      : m_Xcoor{0}, m_Ycoor{0}, m_food{0.0}, m_weights(13, 0.5), m_type(), m_smart(), m_evolve()
    {
        //All done!
    }


private:
    int m_Xcoor;
    int m_Ycoor;
    double m_food;
    std::vector<double> m_weights;
    char m_type;
    char m_smart;
    char m_evolve;

};

#endif // CINE_INDIVIDUAL_H
