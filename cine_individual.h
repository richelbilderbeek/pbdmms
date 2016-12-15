#ifndef CINE_INDIVIDUAL_H
#define CINE_INDIVIDUAL_H

//initialize class individual
class individual
{
public:
    void setPosition(int x, int y);
    int xposition() const { return m_Xcoor; }
    int yposition() const { return m_Ycoor; }
    void food_uptake(double foodintake);
    double return_food() const { return m_food; }
    //std::vector <float> get_weight_vector() { return m_weights; }

private:
    int m_Xcoor;
    int m_Ycoor;
    double m_food;
    //std::vector <float> m_weights;
};

#endif // CINE_INDIVIDUAL_H
