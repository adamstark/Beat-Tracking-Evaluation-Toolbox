
#ifndef TEST_HELPERS
#define TEST_HELPERS

#include <iostream>

//------------------------------------------------------------
inline double randomSign()
{
    int r = rand() % 2;
    return r == 0 ? 1. : -1.;
}

//------------------------------------------------------------
inline double randomPositiveOrNegativeOffset (double minValue, double maxValue)
{
    // generate random number in the range [0, 1]
    double r = (static_cast<double> (rand() % 1000) / 1000.);
    double range = maxValue - minValue;
    double value = minValue + r * range;
    
    // return value with random sign (+/-)
    return value * randomSign();
}

#endif
