#ifndef DNA_H
#define DNA_H

#include <iosfwd>
#include <string>
#include <random>

class dna
{
public:
  dna(const std::string& sequence);

  void change(const size_t i, std::mt19937& rng_engine) noexcept;

  const std::string& get_sequence() const noexcept { return m_sequence; }

  std::string::size_type size() const noexcept { return m_sequence.size(); }

  const char& operator[](const size_t i) const noexcept;
        char& operator[](const size_t i)       noexcept;

private:
  std::string m_sequence;
};

std::ostream& operator<<(std::ostream& os, const dna& d) noexcept;

bool operator==(const dna& lhs, const dna& rhs) noexcept;

#endif // DNA_H
