#include <cmath>
#include <limits>
#include <sstream>
#include <iomanip>
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
  double max, min, delta;

  red = r / 255.0; green = g / 255.0; blue = b / 255.0;  /* r, g, b to 0 - 1 scale */

  max = maximum(red, green, blue); 
  min = minimum(red, green, blue);

  v = max;        /* value is maximum of r, g, b */

  if( max == 0){    /* saturation and hue 0 if value is 0 */ 
     s = 0; 
     h = 0; 
  } else { 
    s = (max - min) / max;           /* saturation is color purity on scale 0 - 1 */

    delta = max - min; 
    if (delta == 0)                         /* hue doesn't matter if saturation is 0 */ 
       h = 0; 
    else { 
      if (red == max)                    /* otherwise, determine hue on scale 0 - 360 */ 
          h = (green - blue) / delta; 
      else if (green == max) 
          h = 2.0 + (blue - red) / delta; 
      else /* (green == max) */ 
          h = 4.0 + (red - green) / delta; 

      h = h * 60.0;                       /* change hue to degrees */
      if (h < 0) 
          h = h + 360.0;                /* negative hue rotated to equivalent positive around color wheel */
    } 
  } 
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

double Utils::round(double number, unsigned int bits) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(bits) << number;
  ss >> number;
  return number;
}

