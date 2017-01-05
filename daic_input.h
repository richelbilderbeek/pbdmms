#ifndef DAIC_INPUT_H
#define DAIC_INPUT_H

#include "daic_input_row.h"

namespace daic {

class input
{
public:
  input(const std::vector<input_row>& table = {}) noexcept;

  const auto& get() const noexcept { return m_table; }

private:
  std::vector<input_row> m_table;

  friend std::ostream& operator<<(std::ostream& os, const input& i) noexcept;
};

/*
Clade_name   Status Missing_species Branching_times
Coccyzus     Non_endemic_MaxAge 0 7.456
Dendroica    Non_endemic 0 0.34
Finches      Endemic 0 3.0282,1.3227,0.8223,0.4286,0.3462,0.245,0.0808,
                     0.0527,0.0327,0.0221,0.118,0.0756,0.0525,0.0322,0.0118
Mimus        Endemic 0 3.958,3.422,2.884,0.459
Myiarchus    Endemic 0 0.855
Progne       Endemic 0 0.086
Pyrocephalus Non_endemic_MaxAge 0 10.285
Zenaida      Endemic 0 3.51
*/
input create_input_article() noexcept;

///cn: clade name
///ms: Missing_species
///+-----------+--------------------+----+---------------------------+
///| cn        | status             | ms | Branching_times           |
///+-----------+--------------------+----+---------------------------+
///| Coccyzus  | Non_endemic_MaxAge |  0 | 7.456                     |
///| Dendroica | Non_endemic        |  0 | 0.34                      |
///| Finches   | Endemic            |  0 | 3.0282,1.3227,0.8223      |
///+-----------+--------------------+----+---------------------------+
input create_input_article_light() noexcept;

///Save the DAISIE input to a file
void save(const input& i, const std::string& filename);

///Will have a terminating '\n'
std::ostream& operator<<(std::ostream& os, const input& i) noexcept;

} //~namespace daic

#endif // DAIC_INPUT_H
