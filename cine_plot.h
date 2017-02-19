#ifndef CINE_PLOT_H
#define CINE_PLOT_H


//initialize class plot
class plot
{
public:
    plot(const int row, const int col) : m_Risk{0.0}, m_Grass{0.0},
        m_adclues{0}, m_Xcoor{col}, m_Ycoor{row}  {}

    void setRisk(double risk);

    double returnRisk() const { return m_Risk; }

    void let_grass_grow(); //? Carrying capacity

    double grass_height() const { return m_Grass; }

    void grass_consumption();

    double return_adclues() const {return m_adclues;}

    void set_adclues(double clues);

    int xposition() const { return m_Xcoor; }
    int yposition() const { return m_Ycoor; }

private:
    ///Probability of prey being caught when also one
    /// (or more?) predator is present in this plot, [0,1]
    double m_Risk;

    ///?Density
    double m_Grass;

    ///Adversary presence clues
    double m_adclues;

    ///Col, only to be read, not made const, because of Rule Of Zero
    int m_Xcoor;

    ///Row, only to be read, not made const, because of Rule Of Zero
    int m_Ycoor;
};

#endif // CINE_PLOT_H
