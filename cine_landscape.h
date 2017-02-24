#ifndef CINE_LANDSCAPE_H
#define CINE_LANDSCAPE_H

#include <vector>
#include "cine_plot.h"

//using landscape = std::vector<std::vector<plot>>;

class landscape
{
public:
 landscape(std::vector<plot> cells, int ncols) : m_cells {cells},
     m_width{ncols}, m_length{static_cast<int>(cells.size()) / ncols} {}
 //Attention, works, but why?
 const plot& operator()(int x, int y) const { return m_cells[y * m_width + x]; }
 plot& operator()(int x, int y) { return m_cells[y * m_width + x]; }

 //void set(const int x, const int y, plot& newplot) { m_cells[y * m_width + x] = newplot; }
 int size() const {return static_cast<int>(m_cells.size());}
 int xsize() const {return m_width;}
 int ysize() const {return m_length;}
 //std::vector<plot> m_cells;

private:
    std::vector<plot> m_cells;
    int m_width;
    int m_length;
};


#endif // CINE_LANDSCAPE_H
