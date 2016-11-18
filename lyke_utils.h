/*===========================================================================================
                                                 utils.h
=============================================================================================

 Miscellaneous utility routines
 
 C++-code accompanying:
 
        authors and G. Sander van Doorn
        title
 
 Written by:
        G. Sander van Doorn
        Groningen Institute for Evolutionary Life Sciences (Gelifes)
        University of Groningen
        the Netherlands
 
 Program version
        xx/xx/20xx	: ...
 
==========================================================================================*/

#ifndef utils_h
#define utils_h

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <string>

extern const double	PI2;
extern const double	PI;
extern const double SQRT2;
extern bool echo_stdclog;

void echo(const std::string&, const bool& = echo_stdclog);
void error(const std::string&, const std::string& = "");
void warning(const std::string&, const std::string& = "");

void dot();
void mark_time(const bool& = false);
void wait_for_return();

void clip_range(double&, const double&, const double&);
void clip_low(double&, const double&);
void clip_high(double&, const double&);
template <class T> inline double sqr(const T &x) { return x * x; };
template <typename T> int sgn(const T& val) {return (T(0) < val) - (val < T(0));}
template <typename T> T max(const T& vala, const T& valb) {return vala > valb ? vala : valb;}
template <typename T> T min(const T& vala, const T& valb) {return vala < valb ? vala : valb;}

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || (defined(__DMC__) && (__DMC__ >= 0x810))
# define CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
# define CURRENT_FUNCTION __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600))
	 || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
	 || (defined(_MSC_VER) && (_MSC_VER >= 1020))
# define CURRENT_FUNCTION __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
# define CURRENT_FUNCTION __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
# define CURRENT_FUNCTION __func__
#else
# define CURRENT_FUNCTION "(unknown)"
#endif

#define verify(condition) \
if (! (condition)) \
{ \
	std::ostringstream oss; \
	oss << "Assertion" << #condition << "failed on line" << __LINE__<< "in file" << __FILE__;\
	error(CURRENT_FUNCTION, oss.str());\
}

#endif //#ifndef utils_h
