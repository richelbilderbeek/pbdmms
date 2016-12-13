#ifndef CINE_INDIVIDUAL_H
#define CINE_INDIVIDUAL_H

//initialize class individual
class individual
{
public:
    void setPosition(int x, int y);

    int xposition() const { return m_Xcoor; }
    int yposition() const { return m_Ycoor; }
    void update_food(double foodintake);
    double return_food() const { return m_food; }
    //void set_fitness(double fit);
    //double return_fitness() const { return m_fitness;  }

private:
    //double m_fitness;
    double m_food;
    int m_Xcoor;
    int m_Ycoor;
    //std::vector <float> wieght;
};

#endif // CINE_INDIVIDUAL_H
