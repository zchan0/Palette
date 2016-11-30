#include <cmath>
#include <limits>
#include <iostream>

#include "utils.h"

#define maximum(x, y, z) ((x) > (y)? ((x) > (z)? (x) : (z)) : ((y) > (z)? (y) : (z))) 
#define minimum(x, y, z) ((x) < (y)? ((x) < (z)? (x) : (z)) : ((y) < (z)? (y) : (z)))

Utils& Utils::getInstance()
{
	static Utils instance;
	return instance;
}

/* following credits to http://stackoverflow.com/a/6930407 */
/* red, green, blue to 0 - 1 scale */
void Utils::RGBtoHSV(double r, double g, double b, double &h, double &s, double &v)
{
  double red, green, blue; 
  double min, max, delta;

  red = r / 255.0; green = g / 255.0; blue = b / 255.0; 

  max = maximum(red, green, blue); 
  min = minimum(red, green, blue);

  v = max;                                // v
  delta = max - min;
  if (delta < 0.00001) {
      s = 0;
      h = 0; // undefined, maybe nan?
      return;
  }
  if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
      s = (delta / max);                  // s
  } else {
      // if max is 0, then r = g = b = 0              
      // s = 0, v is undefined
      s = 0.0;
      h = std::numeric_limits<double>::quiet_NaN(); // its now undefined
      return;
  }
  if( r >= max )                     // > is bogus, just keeps compilor happy
      h = ( g - b ) / delta;        // between yellow & magenta
  else
  if( g >= max )
      h = 2.0 + ( b - r ) / delta;  // between cyan & yellow
  else
      h = 4.0 + ( r - g ) / delta;  // between magenta & cyan
  
  h *= 60.0;                              // degrees

  if( h < 0.0 )
      h += 360.0;
} 

/* r, g, b: percent */
/* h angle(0 ~ 360), s,v(0 ~ 1) */
void Utils::HSVtoRGB(double h, double s, double v, double &r, double &g, double &b)
{
	double hh, p, q, t, ff;
  long   i;

  if (s <= 0.0) {       // < is bogus, just shuts up warnings
      r = v;
      g = v;
      b = v;
  }
  hh = h;
  if (hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i 	= (long)hh;
  ff = hh - i;
  p  = v * (1.0 - s);
  q  = v * (1.0 - (s * ff));
  t  = v * (1.0 - (s * (1.0 - ff)));

  switch(i) {
	  case 0:
	      r = v;
	      g = t;
	      b = p;
	      break;
	  case 1:
	      r = q;
	      g = v;
	      b = p;
	      break;
	  case 2:
	      r = p;
	      g = v;
	      b = t;
	      break;
	  case 3:
	      r = p;
	      g = q;
	      b = v;
	      break;
	  case 4:
	      r = t;
	      g = p;
	      b = v;
	      break;
	  case 5:
	  default:
	      r = v;
	      g = p;
	      b = q;
	      break;
  }
}

