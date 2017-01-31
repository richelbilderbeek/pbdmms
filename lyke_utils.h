#ifndef utils_h
#define utils_h

///Put val in range [min,max].
///If val is below min, val is set to min
///If val is above max, val is set to max
///Else val is unchanged
void clip_range(double &val, const double min, const double max);

#endif //#ifndef utils_h
