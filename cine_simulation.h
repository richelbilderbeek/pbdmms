#ifndef CINE_SIMULATION_H
#define CINE_SIMULATION_H

void do_simulation();

//initialize class plot
class plot
{
public:
    void setRisk();
    void setPosition(int iX, int iY);
    void increaseGrass();
    double dGrsupply() const { return dGrass; }
    void grazing();
    double returnRisk() const { return dRisk; }
    int xposition() const { return iXcoor; }
    int yposition() const { return iYcoor; }

private:
    double dRisk;
    double dGrass;
    int iXcoor;
    int iYcoor;
};

#endif // CINE_SIMULATION_H
