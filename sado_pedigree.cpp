#include "sado_pedigree.h"
//#include "sado_indiv_graph.h"
//#include "sado_id.h"
//#include "sado_individual_vertex.h"

#include <cassert>
#include <stdexcept>
sado::pedigree::pedigree()
{

}

void sado::pedigree::add(const indiv& i)
{
  /*const vert_desc vd = add_individual_vertex(i, m_g);
  std::pair<indiv,vert_desc>indiv_vd_pair {i, vd};
  m_v.push_back(indiv_vd_pair);

  for(std::pair<indiv,vert_desc> pair : m_v)
  {
    int parent_count{0};
    if (pair.first.get_id() == i.get_father_id() || pair.first.get_id() == i.get_mother_id())
    {
      boost::add_edge(pair.second, vd, m_g);
      ++parent_count;
      if (parent_count == 2)
        break;
    }
  }*/

  m_v.push_back(i);

}

bool sado::pedigree::are_related(const indiv& a, const indiv& b)
{
  if (a.get_id() == b.get_id()) { throw std::invalid_argument("a and b are the same individual");}

  if (a.get_id() > b.get_id())
      return sado::pedigree::check_parents_for_id(a, b.get_id());
  else
      return sado::pedigree::check_parents_for_id(b, a.get_id());
}

bool sado::pedigree::check_parents_for_id(const sado::indiv& a, const sado::id& idnum)
{
  if (a.get_father_id() == create_null_id() && a.get_mother_id() == create_null_id())
  {
    return false;
  }
  else if (a.get_father_id() < idnum && a.get_mother_id() < idnum)
    return false;
  else if (a.get_father_id() == idnum || a.get_mother_id() == idnum)
    return true;

  const indiv father = get_indiv_from_id(a.get_father_id());
  const indiv mother = get_indiv_from_id(a.get_mother_id());
  return check_parents_for_id(father, idnum) || check_parents_for_id(mother, idnum);
}

sado::indiv sado::pedigree::get_indiv_from_id(const sado::id& idnum)
{
  if (m_v.empty()) throw std::invalid_argument("pedigree is empty");
  for (const indiv individual : m_v)
  {
    if (individual.get_id() == idnum)
      return individual;
  }
  throw std::invalid_argument("No individual found");
}

bool sado::pedigree::empty() const noexcept
{
  return m_v.empty();
}

bool sado::operator==(const pedigree& lhs, const pedigree& rhs) noexcept
{
  return lhs.m_v == rhs.m_v;
}

bool sado::operator!=(const pedigree& lhs, const pedigree& rhs) noexcept
{
  return !(lhs == rhs);
}
