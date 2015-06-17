/*
// Copyright (c) 2012-2015 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef __DEF_HOA_MAX_MATHS__
#define __DEF_HOA_MAX_MATHS__

#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string>
#include <assert.h>
#include <string.h>

#ifdef _WINDOWS
static inline double round(double val)
{    
    return floor(val + 0.5);
}
#endif

//! The high order ambisonic namespace.
/**
 This namespace have all the standard methods and functions necessary for ambisonic processing.
*/
namespace hoa
{
#ifdef _MSC_VER
	inline double round(double val)
	{
		return floor(val + 0.5);
	}
#endif

	inline double radToDeg(const double radian)
    {
        return radian * 360. / HOA_2PI;
    }

    inline double degToRad(const double degree)
    {
        return degree / 360. * HOA_2PI;
    }
    
    inline double atodb(const double amp)
    {
        return (amp <= 0.) ? -999.f : (20. * log10(amp));
    }

    inline double dbtoa(const double dB)
    {
        return pow(10., dB * 0.05);
    }

	inline double safediv(const double num, const double denom)
    {
        return denom == 0. ? 0. : num/denom;
    }
    
    inline double wrap(const double value, const double low, const double high)
    {
        const double increment = high - low;
        double new_value = value;
        while(new_value < low)
        {
            new_value += increment;
        }
        while(new_value > high)
        {
            new_value -= increment;
        }
        return new_value;
    }

	inline double scale(const double in, const double inlow, const double inhigh, const double outlow, const double outhigh, const double power)
    {
        double value;
        double inscale = safediv(1., inhigh - inlow);
        double outdiff = outhigh - outlow;

        value = (in - inlow) * inscale;
        if (value > 0.0)
            value = pow(value, power);
        else if (value < 0.0)
            value = -pow(-value, power);
        value = (value * outdiff) + outlow;

        return value;
    }

    inline double scale(const double in, const double inlow, const double inhigh, const double outlow, const double outhigh)
    {
        return ( (in - inlow) * safediv(1., inhigh - inlow) * (outhigh - outlow) ) + outlow;
    }

	template <typename T> inline bool isInside(const T val, const T v1, const T v2)
	{
        return (v1 <= v2) ? (val >= v1 && val <= v2) : (val >= v2 && val <= v1);
	}

	inline bool isInsideRad(const double radian, const double loRad, const double hiRad)
	{
        return isInside<double>(Math<double>::wrap_twopi(radian-loRad), 0, Math<double>::wrap_twopi(hiRad-loRad));
	}
    
    inline double distance(double x1, double y1, double x2, double y2)
    {
        return sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
    }
}

#endif
