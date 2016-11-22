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

///Put val in range [min,max].
///If val is below min, val is set to min
///If val is above max, val is set to max
///Else val is unchanged
void clip_range(double &val, const double min, const double max);

#endif //#ifndef utils_h
