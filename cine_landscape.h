#ifndef CINE_LANDSCAPE_H
#define CINE_LANDSCAPE_H

#include <vector>
#include "cine_plot.h"

using landscape = std::vector<std::vector<plot>>;

//class landscape
//{
//public:
// landscape(std::vector<plot> cells) : m_cells {cells}, m_width{} {}
// const plot& operator()(int x, int y) const { return m_cells[y * m_width + x]; }
// int size() const {return static_cast<int>(m_cells.size());}

//private:
//    std::vector<plot> m_cells;
//    int m_width;
//};


#endif // CINE_LANDSCAPE_H
