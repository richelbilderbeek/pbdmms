#ifndef SADO_ERASURE_METHOD_H
#define SADO_ERASURE_METHOD_H

#include <string>

namespace sado {

enum class erasure_method { erase, swap };

erasure_method to_erasure_method(const std::string& s);

} //~namespace sado

#endif // SADO_ERASURE_METHOD_H
