#ifndef CINE_SIMULATION_H
#define CINE_SIMULATION_H

void do_simulation();

void create_ANN();

//initialize class plot
class plot
{
public:
    void setRisk();
    void setPosition(int x, int y);
    void increaseGrass();
    double dGrsupply() const { return m_Grass; }
    void grazing();
    double returnRisk() const { return m_Risk; }
    int xposition() const { return m_Xcoor; }
    int yposition() const { return m_Ycoor; }

private:
    double m_Risk;
    double m_Grass;
    int m_Xcoor;
    int m_Ycoor;
};

#endif // CINE_SIMULATION_H
